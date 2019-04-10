#ifndef VELODYNECAPTURE_H
#define VELODYNECAPTURE_H

#include <string>
#include <sstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <vector>
#include <cassert>
#include <cstdint>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <iostream>
#include <QDebug>

#define HAVE_BOOST
#define PUSH_SINGLE_PACKETS
#define NO_EMPTY_RETURNS
//#define HAVE_PCAP

#ifdef HAVE_BOOST
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#endif
#ifdef HAVE_PCAP
#include <pcap.h>
#endif
#define EPSILON 0.001

namespace velodyne
{
    struct Laser
    {
        double azimuth;
        double vertical;
        float distance;
        unsigned char intensity;
        unsigned char id;
        long long time;

        const bool operator < ( const struct Laser& laser ) {
            if (azimuth == laser.azimuth) {
                return id < laser.id;
            }
            else {
                return azimuth < laser.azimuth;
            }
        }
    };

    class VelodyneCapture
    {
    protected:
    #ifdef HAVE_BOOST
    boost::asio::io_service ioservice;
    boost::asio::ip::udp::socket* socket = nullptr;
    boost::asio::ip::address address;
    unsigned short port = 2368;
    #endif
    std::thread* thread = nullptr;
    std::atomic_bool run = { false };
    std::mutex mutex;
    std::queue<std::vector<Laser>> queue;

    int MAX_NUM_LASERS;
    std::vector<double> lut;
    double time_between_firings;
    double time_half_idle;
    double time_total_cycle;

    static const int LASER_PER_FIRING = 32;
    static const int FIRING_PER_PKT = 12;

#pragma pack(push, 1)
    typedef struct LaserReturn
    {
        uint16_t distance;
        uint8_t intensity;
    } LaserReturn;
#pragma pack(pop)

#pragma pack(push, 1)
    struct FiringData
    {
        uint16_t blockIdentifier;
        uint16_t rotationalPosition;
        LaserReturn laserReturns[LASER_PER_FIRING];
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct DataPacket
    {
        FiringData firingData[FIRING_PER_PKT];
        uint32_t gpsTimestamp;
        uint8_t mode;
        uint8_t sensorType;
    };
#pragma pack(pop)
    public:
        VelodyneCapture()
        {

        };
#ifdef HAVE_BOOST
        VelodyneCapture( const boost::asio::ip::address& address, const unsigned short port = 2368)
        {
            open( address, port);
        }
#endif

        // Deconstructor
        ~VelodyneCapture()
        {
            close();
        }
#ifdef HAVE_BOOST
        // Open Direct capture from sensor
    const bool open( const boost::asio::ip::address& address, const unsigned short port = 2368)
    {
        if ( isRun() ){
            close();
        }
        // Set ip address and port
        this->address = ( !address.is_unspecified())? address : boost::asio::ip::address::from_string(
                                                          "255.255.255.255");
        this->port = port;

        // Create socket
        try {
            socket = new boost::asio::ip::udp::socket( ioservice, boost::asio::ip::udp::endpoint( this->
                                                                                                  address, this->port));

        }
        catch( ... ) {
            delete socket;
            socket = new boost::asio::ip::udp::socket( ioservice, boost::asio::ip::udp::endpoint(
                                                           boost::asio::ip::address_v4::any(), this->port));
        }

        // Start IO-Service
        try {
            ioservice.run();
        }
        catch ( const std::exception& e ) {
            std::cerr << e.what() << std::endl;
            return false;
        }
        // Start capture thread
        run = true;
        thread = new std::thread( std::bind ( &VelodyneCapture::captureSensor, this));
        return true;
    };
#endif


    // Check open
    const bool isOpen()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return (
            #if defined( HAVE_BOOST) || defined( HAVE_PCAP )
            #ifdef HAVE_BOOST
                    (socket && socket->is_open())
            #endif
            #if defined( HAVE_BOOST) && defined( HAVE_PCAP )
                    ||
            #endif
            #ifdef HAVE_PCAP
                    pcap != nullptr
            #endif
            #else
                    false
            #endif
            );
    };

    // Check run
    const bool isRun()
    {
        //Returns True when thread is running or queue is not empty
        std::lock_guard<std::mutex> lock( mutex );
        return (run || !queue.empty() );
    }

    // Close capture
    void close()
    {
        run = false;
        // Close Capture thread
        if ( thread && thread->joinable() ) {
            thread->join();
            thread->~thread();
            delete thread;
            thread = nullptr;
        }

        std::lock_guard<std::mutex> lock( mutex );

#ifdef HAVE_BOOST
        // Close socket
        if (socket && socket->is_open() ) {
            socket->close();
            delete socket;
            socket = nullptr;
        }

        // Stop IO-service
        if ( ioservice.stopped() ) {
            ioservice.stop();
            ioservice.reset();
        }
#endif
#ifdef HAVE_PCAP
        //Close PCAP
        if (pcap) {
            pcap_close(pcap);
            pcap = nullptr;
            filename = "";
        }
#endif
        // Clear queue
        std::queue<std::vector<Laser>>().swap( queue );
    };

    // Retrieve capture data
    void retrieve( std::vector<Laser> & lasers, const bool sort = false )
    {
        // Pop one rotation data from queue
        if (mutex.try_lock() ) {
            if (!queue.empty() ) {
                lasers = std::move( queue.front() );
                queue.pop();
                if ( sort ) {
                    std::sort( lasers.begin(), lasers.end() );
                }
            }
            mutex.unlock();
        }
    };

    // Operator Retrieve capture data with queue
    void operator >> (std::vector<Laser> & lasers)
    {
        // Retrieve capture data
        retrieve( lasers, false);
    };

    size_t getQueueSize()
    {
        std::lock_guard<std::mutex> lock( mutex );
        return queue.size();
    }

    private:

    void parseDataPacket( const DataPacket* packet, std::vector<Laser>& lasers, double& last_azimuth)
    {
        if ( packet->sensorType != 0x21 && packet->sensorType != 0x22 && packet->sensorType != 0x24) {
            //qDebug() << packet->sensorType;
            throw( std::runtime_error( " This sensor is not supported"));
        }
        if ( packet->mode != 0x37 && packet->mode != 0x38) {
            throw( std::runtime_error( "Sensor can't be set in dual return mode"));
        }

        // Retrieve unix time ( microseconds)
        const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        const std::chrono::microseconds epoch = std::chrono::duration_cast<std::chrono::microseconds>( now.time_since_epoch());
        const long long unixtime = epoch.count();

        // Azimuth delta is the angle from one firing sequence to the next one
        double azimuth_delta = 0.0;
        if ( packet->firingData[1].rotationalPosition < packet->firingData[0].rotationalPosition ) {
            azimuth_delta = ( ( packet->firingData[1].rotationalPosition + 36000 ) - packet->firingData
                    [0].rotationalPosition);
        }
        else {
            azimuth_delta = ( packet->firingData[1].rotationalPosition - packet->firingData[0].rotationalPosition);
        }

        // Processing packet
        for ( int firing_index = 0; firing_index < FIRING_PER_PKT; firing_index++ ) {
            // Retrieve firing data
            const FiringData firing_data = packet->firingData[firing_index];
            for ( int laser_index = 0; laser_index < LASER_PER_FIRING; laser_index++ ) {
                // Retrieve rotation azimuth
                double azimuth = static_cast<double>(firing_data.rotationalPosition);
                double laser_relative_time = LASER_PER_FIRING * time_between_firings + time_half_idle *
                        (laser_index / MAX_NUM_LASERS);
                azimuth += azimuth_delta * laser_relative_time / time_total_cycle;

                // Reset rotation azimuth
                if ( azimuth >= 36000 )
                {
                    azimuth -= 36000;
                }

                // Complete Retrieve capture one rotation data
#ifndef PUSH_SINGLE_PACKETS
                if (last_azimuth > azimuth) {
                    //Push one rotation data to queue
                    mutex.lock();
                    queue.push( std::move( lasers ));
                    mutex.unlock();
                    lasers.clear();
                }
#endif
#ifdef NO_EMPTY_RETURNS
                if (firing_data.laserReturns[laser_index].distance < EPSILON){
                    continue;
                }
#endif
                Laser laser;
                laser.azimuth = azimuth / 100.0f;
                laser.vertical = lut[laser_index % MAX_NUM_LASERS];
#ifdef USE_MILLIMETERS
     laser.distance = static_cast<float>(firing_data.laserReturns[laser_index].distance ) * 2.0f;
#else
                laser.distance = static_cast<float>(firing_data.laserReturns[laser_index].distance) * 2.0f / 10.0f;
#endif
                laser.intensity = firing_data.laserReturns[laser_index].intensity;
                laser.id = static_cast<unsigned char> ( laser_index % MAX_NUM_LASERS);
#ifdef HAVE_GPSTIME
      laser.time = packet->gpsTimestamp + static_cast<long long>( laser_relative_time);
#else
                laser.time = unixtime + static_cast<long long>(laser_relative_time );
#endif
                lasers.push_back( laser );
                //Update last rotation azimuth
                last_azimuth = azimuth;
            }
        }
#ifdef PUSH_SINGLE_PACKETS
        // Push packet after processing
        mutex.lock();
        queue.push( std::move( lasers ));
        mutex.unlock();
        lasers.clear();
    };

#endif
    #ifdef HAVE_BOOST
    // Capture thread from sensor
    void captureSensor()
    {
        double last_azimuth = 0.0;
        std::vector<Laser> lasers;
        unsigned char data[1500];
        boost::asio::ip::udp::endpoint sender;

        while( socket->is_open() && ioservice.stopped() && run ) {
            // Receive Packet
            boost::system::error_code error;
            const size_t length = socket->receive_from( boost::asio::buffer( data, sizeof( data)), sender, 0,
                                                        error);
            if ( error == boost::asio::error::eof) {
                break;
            }

            // Check IP-address and port
            if ( sender.address() != address && sender.port() != port) {
                continue;
            }

            // Check packet data size
            // Data blocks ( 100 bytes * 12 blocks ) + Time stamp ( 4 bytes) + factory ( 2 bytes)
            if ( length != 1206 ) {
                continue;
            }

            // Convert to datapacket structure
            // sensor type 0x21 is HDL-32E, 0x22 is VLP-16
            // sensor type 0x24 (36) Puck Hi-Res
            const DataPacket* packet = reinterpret_cast<const DataPacket*>( data );
            parseDataPacket( packet, lasers, last_azimuth );
        }
        run = false;
    };
    #endif

#ifdef HAVE_PCAP
        //Capture Thread from PCAP

#endif
    };

    class VLP16Capture : public VelodyneCapture
    {
    private:
        static const int MAX_NUM_LASERS = 16;
        const std::vector<double> lut = { -15.0, 1.0, -13.0, 3.0, -11.0, 5.0, -9.0, 7.0, -7.0, 9.0, -5.0, 11.0,
                                        -3.0, 13.0, -1.0, 15.0 };
        const double time_between_firings = 2.304;
        const double time_half_idle = 18.432;
        const double time_total_cycle = 55.296 * 2;

    public:
        VLP16Capture() : VelodyneCapture()
        {
            initialize();
        };
#ifdef HAVE_BOOST
      VLP16Capture( const boost::asio::ip::address& address, const unsigned short port = 2368 ) : VelodyneCapture(address, port)
      {
          initialize();
      };

#endif

      ~VLP16Capture()
      {

      }

    private:
        void initialize()
        {
            VelodyneCapture::MAX_NUM_LASERS = MAX_NUM_LASERS;
            VelodyneCapture::lut = lut;
            VelodyneCapture::time_between_firings = time_between_firings;
            VelodyneCapture::time_half_idle = time_half_idle;
            VelodyneCapture::time_total_cycle = time_total_cycle;
        };
    };



    class VLP16HiResCapture : public VelodyneCapture
    {
    private:
        static const int MAX_NUM_LASERS = 16;
        const std::vector<double> lut = { -10.0, 0.67, -8.67, 2.00, -7.33, 3.33, -6.00, 4.67, -4.67, 6.00, -3.33,
                                        7.33, -2.00, 8.67, -0.67, 10.00};
        const double time_between_firings = 2.304;
        const double time_half_idle = 18.432;
        const double time_total_cycle = 55.296 * 2;

    public:
        VLP16HiResCapture() : VelodyneCapture()
        {
            initialize();
        };
#ifdef HAVE_BOOST
      VLP16HiResCapture( const boost::asio::ip::address& address, const unsigned short port = 2368 ) : VelodyneCapture(address, port)
      {
          initialize();
      };

#endif

      ~VLP16HiResCapture()
      {

      }

    private:
        void initialize()
        {
            VelodyneCapture::MAX_NUM_LASERS = MAX_NUM_LASERS;
            VelodyneCapture::lut = lut;
            VelodyneCapture::time_between_firings = time_between_firings;
            VelodyneCapture::time_half_idle = time_half_idle;
            VelodyneCapture::time_total_cycle = time_total_cycle;
        };
    };
}
#endif // VELODYNECAPTURE_H
