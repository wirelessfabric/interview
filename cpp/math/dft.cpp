// SPDX-License-Identifier: MIT
// x86_64-pc-linux-gnu-gcc-10.2.0 -std=c++2a -O3 -lstdc++ -lm dft.cpp -o dft

#include <iostream>
#include <math.h>
#include <complex>
#include <iomanip>

static struct {
    int runs;
    bool verbose;
    bool naive;
    bool exp;
    bool dbl;
} config;

template <typename T>
T slice(int n) {
    constexpr T pie = 2 * M_PI;
    return pie / static_cast<T>(n);
}

template <typename T>
T *dftrow_cos(T *v, T dw, int n) {
    T w = 0;
    if (v && n > 0 ) do {
        *v++ = (T)cos(w);
        w += dw;
    } while (--n);
    return v;
}

template <typename T>
T *dftrow_nsin(T *v, T dw, int n) {
    T w = 0;
    if (v && n > 0 ) do {
        *v++ = (T)-sin(w);
        w += dw;
    } while (--n);
    return v;
}

template <typename T>
void dftmat(T *v, T theta, T *(*f)(T *, T, int), const int n) {
    auto dw = (T)0;
    if (v && f) for (auto l=0; l < n; ++l) {
        v = f(v, dw, n);
        dw += theta;
    }
}

template <typename T>
void dftmat_cosnsin(T *r, T *i, const int n) {
    const auto theta = slice<T>(n);
    dftmat(r, theta, dftrow_cos, n);
    dftmat(i, theta, dftrow_nsin, n);
}

template <typename T>
void dftmat_exp(T *r, T *i, const int n) {
    std::complex<T> x(0, -slice<T>(n));
    std::complex<T> w = std::exp(x);
    for (auto k=0; k < n; ++k) {
        for (auto l=0; l < n; ++l) {
            std::complex<T> c(std::pow(w, k*n));
            *r++ = std::real(c);
            *i++ = std::imag(c);
        }
    }
}

template <typename T>
void dft(T *sigr,  T *sigi, T *dftr, T *dfti, T *matr, T *mati, const int n) {
    const auto theta = slice<T>(n);

    auto dw = (T)0;
    for (auto k=0; k < n; ++k) {
        auto w = (T)0;
        auto sumr = (T)0;
        auto sumi = (T)0;
        auto spr = sigr;
        auto spi = sigi;
        for (auto l=0; l < n; ++l) {
            auto mr = *matr++;
            auto mi = *mati++;
            auto sr = *spr++;
            auto si = *spi++;
            sumr += sr * mr - si * mi;
            sumi += si * mr + sr * mi;
            w += dw;
        }
        *dftr++ = sumr;
        *dfti++ = sumi;
        dw += theta;
    }
}

template <typename T>
void dftexp(std::complex<T> *sig, std::complex<T> *dft,
            T *matr, T *mati, const int N)
{
    const auto theta = slice<T>(N);

    auto dw = (T)0;
    for (auto i=0, k=0; k < N; ++k) {
        auto w = (T)0;
        auto sum = std::complex<T>(0, 0);
        for (auto n=0; n < N; ++n, ++i) {
            sum += sig[n] * std::complex<T>{(T)matr[i], (T)mati[i]};
            w += dw;
        }
        dft[k] = sum;
        dw += theta;
    }
}

template <typename T>
void dftexp(std::complex<T> *sig, std::complex<T> *dft, const int N) {
    std::complex<T> x(0, -slice<T>(N));
    std::complex<T> w = std::exp(x);

    for (auto k=0; k < N; k++) {
        std::complex<T> sum = std::complex<T>(0, 0);
        for (auto n=0; n < N; n++)
            sum += sig[n] * std::complex<T>{std::pow(w, k*n)};
        dft[k] = sum;
    }
}

template <typename T>
void dftcosnsin(std::complex<T> *sig, std::complex<T> *dft,
                T *cosine, T *sine, const int N)
{
    const auto theta = slice<T>(N);

    auto dw = (T)0;
    for (auto i=0, k=0; k < N; ++k) {
        auto w = (T)0;
        auto sum = std::complex<T>(0, 0);
        for (auto n=0; n < N; ++n, ++i) {
            sum += sig[n] * std::complex<T>{(T)cosine[i], (T)sine[i]};
            w += dw;
        }
        dft[k] = sum;
        dw += theta;
    }
}

template <typename T>
void dftcosnsin(std::complex<T> *sig, std::complex<T> *dft, const int N) {
    const auto theta = slice<T>(N);

    auto dw = (T)0;
    for (auto k=0; k < N; k++) {
        auto w = (T)0;
        auto sum = std::complex<T>(0, 0);
        for (auto n=0; n < N; n++) {
            sum += sig[n] * std::complex<T>{(T)cos(w), (T)-sin(w)};
            w += dw;
        }
        dft[k] = sum;
        dw += theta;
    }
}

template<typename T>
void dft(const int N) {
    const auto theta = slice<T>(N);
    double cycles = 3.0;
    double phase = 0.0;

    if (config.naive) {
        auto mem = new std::complex<T>[(size_t)(2*N)];
        if (mem) {
            auto sigc = mem;
            auto dftc = &sigc[N];

            for (auto n=0; n < N; ++n)
                sigc[n] = {(T)cos(theta * cycles * static_cast<T>(n) + phase), (T)0};

            if (config.verbose) {
                std::cout << "****" << std::endl;
                std::cout << "First 6 samples of signal..." << std::endl;
                std::cout << "\nk\t" << std::setw(12) << "re\t" << std::setw(12) << "im" << std::endl;
                for (auto n=0; n < 6; n++) {
                    std::cout << n << "\t"
                            << std::setw(12) << sigc[n].real() / static_cast<T>(N) << "\t"
                            << std::setw(12) << sigc[n].imag() / static_cast<T>(N) << std::endl;
                }
            }

            if (config.exp)
                for (auto r=0; r < config.runs; ++r)
                    dftexp<T>(sigc, dftc, N);
            else
                for (auto r=0; r < config.runs; ++r)
                    dftcosnsin<T>(sigc, dftc, N);

            if (config.verbose) {
                std::cout << "****" << std::endl;
                std::cout << "First 6 samples of output..." << std::endl;
                std::cout << "\nk\t" << std::setw(12) << "re\t" << std::setw(12) << "im" << std::endl;
                for (auto k=0; k < 6; k++) {
                    std::cout << k << "\t"
                            << std::setw(12) << dftc[k].real() / static_cast<T>(N) << "\t"
                            << std::setw(12) << dftc[k].imag() / static_cast<T>(N) << std::endl;
                }
            }

            delete mem;
        }
    } else {
        auto mem = new T[(size_t)(4+2*N)*N];
        if (mem) {
            auto sigr = mem;
            auto sigi = &sigr[N];
            auto dftr = &sigi[N];
            auto dfti = &dftr[N];
            auto matr = &dfti[N];
            auto mati = &matr[N*N];

            for (auto n=0; n < N; n++) {
                sigr[n] = cos(theta * cycles * static_cast<T>(n) + phase);
                sigi[n] = (T)0;
            }

            if (config.verbose) {
                std::cout << "****" << std::endl;
                std::cout << "First 6 samples of signal..." << std::endl;
                std::cout << "\nk\t" << std::setw(12) << "re\t" << std::setw(12) << "im" << std::endl;
                for (auto n=0; n < 6; n++) {
                    std::cout << n << "\t"
                            << std::setw(12) << sigr[n] / static_cast<T>(N) << "\t"
                            << std::setw(12) << sigi[n] / static_cast<T>(N) << std::endl;
                }
            }

            if (config.exp)
                dftmat_exp<T>(matr, mati, N);
            else
                dftmat_cosnsin<T>(matr, mati, N);

            for (auto r=0; r < config.runs; ++r)
                dft<T>(sigr, sigi, dftr, dfti, matr, mati, N);

            if (config.verbose) {
                std::cout << "****" << std::endl;
                std::cout << "First 6 samples of output..." << std::endl;
                std::cout << "\nk\t" << std::setw(12) << "re\t" << std::setw(12) << "im" << std::endl;
                for (auto k=0; k < 6; k++) {
                    std::cout << k << "\t"
                            << std::setw(12) << dftr[k] / static_cast<T>(N) << "\t"
                            << std::setw(12) << dfti[k] / static_cast<T>(N) << std::endl;
                }
            }

            delete mem;
        }
    }
}

int usage(int argc, char **argv, int retcode) {
    switch (retcode) {
    case -1: std::cout << "Empty - option" << std::endl; break;
    case -2: std::cout << "Invalid - option" << std::endl; break;
    default: break;
    }
    return retcode;
}

int main(int argc, char **argv) {
    config.runs = 1;
    config.verbose = false;
    config.naive = false;
    config.exp = false;
    config.dbl = false;
    for (auto i=1; i < argc; i++) {
        if (argv[i][0] != '-')
            config.runs = (int)strtol(argv[i], nullptr, 0);
        else if (argv[i][1] == '\0')
            return usage(argc, argv, -1);
        else switch ((int)argv[i][1]) {
            case 'v': config.verbose = true; break;
            case 'e': config.exp = true; break;
            case 'n': config.naive = true; break;
            case 'd': config.dbl = true; break;
            default: return usage(argc, argv, -2);
        }
    }

    if (config.verbose) {
        if (config.naive)
            std::cout << "naive ";
        else
            std::cout << "optimize ";
        
        if (config.exp)
            std::cout << "exp ";
        else
            std::cout << "cosnsin ";

        if (config.dbl)
            std::cout << "double" << std::endl;
        else
            std::cout << "float" << std::endl;
    }

    constexpr auto N = 640;
    if (config.dbl)
        dft<double>(N);
    else
        dft<float>(N);

    return 0;
}
