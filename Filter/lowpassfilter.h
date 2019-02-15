#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H
#include <Common/common.h>
#include <QtMath>
class LowpassFilter
{
public:
    LowpassFilter(int sampling_freq)
    {
        w = 0;
        T = 1.f / sampling_freq;

        y_k1 = 0;
        u_k1 = 0;
    }

    inline void Init(float cutoff_freq)
    {
        if (cutoff_freq <= 0 || 0.4f < cutoff_freq * T) {
            w = 0;
            return;
        }
        w = 2 / T * qTan(M_PI * cutoff_freq * T);

        a1 = (2 - w*T) / (2 + w*T);
        b1 = (    w*T) / (2 + w*T);
    }

    inline float Filter(float u_k)
    {
        if (w == 0) return u_k;
        float y_k = a1*y_k1 + b1*(u_k + u_k1);

        y_k1 = y_k;
        u_k1 = u_k;

        return y_k;
    }

    inline void Reset()
    {
        y_k1 = 0;
        u_k1 = 0;
    }
private:
    float w;//Cutoff freq
    float T;//Sampling period

    float y_k1; //y[k-1]
    float u_k1; //u[k-1]

    float a1;
    float b1;

};

#endif // LOWPASSFILTER_H
