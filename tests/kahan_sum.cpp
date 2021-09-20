#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <assert.h>

float random_float() {
  float result = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return result;
}

void FillVector(std::vector<float> &values, size_t size) {

  // We want to read whole chunk of data
  values.resize(size);
  float *raw_array = values.data();

  // ios::app is required for the file to be created with std::ios::in
  std::fstream file("data.in", std::ios::in | std::ios::out | std::ios::binary |
                                   std::ios::app);
  size_t readable = 0;
  size_t element_read = 0;

  if (file.is_open()) {
    file.seekg(0, std::ios::end);
    readable = file.tellg();
    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char *>(raw_array), readable);
    readable /= sizeof(float);
    element_read = readable;
  }

  if (element_read == size)
    return;

  std::cout << "Read " << element_read << " elements, expected " << size
            << std::endl;
  // Generate the missing values
  for (int i = element_read; i < size; i++) {
    float value = random_float();
    values[i] = value;
  }
  // write the missing bits
  file.write(reinterpret_cast<char *>(raw_array + readable),
             (size - readable) * sizeof(float));
  file.close();
}

template <typename iterator>
void KahanSum(iterator begin_iterator, iterator end_iterator, float *result) {
  float sum = 0.0f;
  float c = 0.0f;
  for (iterator it = begin_iterator; it != end_iterator; it++) {
    float y = *it - c;
    float t = sum + y;
    c = (t - sum) - y;
    sum = t;
  }
  *result = sum;
}

float RunKahanSum(const std::vector<float> &values, size_t nThreads) {
  assert(values.size() > 0 && "invalid thread number");

  size_t size = values.size();
  size_t chunk_size = size / nThreads;

  using iterator = std::vector<float>::const_iterator;

  std::vector<std::thread> threads;
  std::vector<float> sums(nThreads);

  for (size_t i = 0; i < nThreads - 1; i++) {

    iterator begin_iterator = values.begin() + i * chunk_size;
    iterator end_iterator = values.begin() + (i + 1) * chunk_size;

    threads.push_back(std::thread(KahanSum<iterator>, begin_iterator,
                                  end_iterator, &sums[i]));
  }
  KahanSum(values.begin() + (nThreads - 1) * chunk_size, values.end(),
           &sums[nThreads - 1]);

  for (size_t i = 0; i < nThreads - 1; i++)
    threads[i].join();

  float sum = 0;
  for (size_t i = 0; i < nThreads; i++)
    sum += sums[i];

  return sum;
}

constexpr size_t nSamples = 1000000;
constexpr size_t nMaxThreads = 8;
constexpr size_t nRun = 10;

void Sample() {
  using namespace std::chrono;

  std::vector<float> values;

  // We conserve the same data for each runs
  auto begin = high_resolution_clock::now();
  FillVector(values, nSamples);
  auto end = high_resolution_clock::now();
  std::cout << "Filled data vector in "
            << duration_cast<milliseconds>(end - begin).count() << "ms\n\n";

  std::cout << "Running kahan sum with " << nSamples << " elements ";
  std::cout << "and " << nMaxThreads << " max threads (" << nRun
            << " runs):" << std::endl;

  std::vector<float> results;
  milliseconds total_duration = 0ms;
  for (size_t i = 1; i <= nMaxThreads; i++) {
    begin = high_resolution_clock::now();
    for (size_t j = 0; j < nRun; j++) {
      results.push_back(RunKahanSum(values, i));
    }
    end = high_resolution_clock::now();

    milliseconds duration = duration_cast<milliseconds>(end - begin);
    total_duration += duration;
    std::cout << "\t" << i << " threads: " << duration.count() / 10 << "ms\n";
  }

  for (size_t i = 1; i < nMaxThreads; i++) {
    if (results[i] != results[0]) {
      std::cerr << "Multiple runs yield different results !\n";
      exit(1);
    }
  }
  std::cout << "Test ended, total duration: " << total_duration.count() << "ms\n";
}

int main(int argc, char **argv) {

  srand(time(nullptr));
  Sample();

  return 0;
}