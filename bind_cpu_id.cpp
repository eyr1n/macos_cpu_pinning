#include <Accelerate/Accelerate.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/sysctl.h>
#include <vector>

int main() {
  // 指定したコアに束縛
  // 0-5: E-core, 6-9: P-core
  int bind_cpu_id = 6;
  if (sysctlbyname("kern.sched_thread_bind_cpu", nullptr, nullptr, &bind_cpu_id,
                   sizeof(bind_cpu_id)) != 0) {
    std::cerr << "sysctlbyname: " << std::strerror(errno) << std::endl;
    return 1;
  }

  // BLASをシングルスレッドで実行
  if (BLASSetThreading(BLAS_THREADING_SINGLE_THREADED) != 0) {
    std::cerr << "BLASSetThreading failed" << std::endl;
    return 1;
  }

  // 単一コアに束縛したい処理
  constexpr int N = 512;
  std::vector<double> A(N * N, 1.0);
  std::vector<double> B(N * N, 2.0);
  std::vector<double> C(N * N, 0.0);

  for (int i = 0; i < 10000; ++i) {
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0,
                A.data(), N, B.data(), N, 0.0, C.data(), N);
  }

  return 0;
}
