// TODO: redo with fork example
//
// Basically, wanted to dmake a very simple example, remind myself of
// what tends to be annoying when doing multicore programming, and
// what patterns can benefit us a little more.
//
#include <cstdint>
#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <utility> // pair, make_pair
#include <vector>

// C - getopt
#include <unistd.h>

template <typename T>
class MTVector {
public:
  MTVector() : items_({}), mut_({}) {}
  ~MTVector() {}
  MTVector(MTVector& other) = delete;
  MTVector(MTVector&& other) = delete;
  MTVector& operator=(MTVector& other) = delete;
  MTVector& operator=(MTVector&& other) = delete;

  void Add(T t) {
    std::lock_guard lock(mut_);
    items_.push_back(t);
  }

  const std::vector<T>& GetUnsafe() const {
    return items_;
  }

private:
  std::vector<T> items_;
  std::mutex mut_;
};

/**
 * a glorified calculator
 *
 * should take a number of elements and suggest best partitioning
 * between threads
 *
 * TODO: this might be useful enough to factor out on some common
 *   module
 */
class Granulator {
public:
  explicit Granulator(size_t num_elements) :
    threshold_(128),
    num_elements_(num_elements),
    num_threads_(std::thread::hardware_concurrency()),
    grain_(num_elements_ / num_threads_),
    step_(0)
  {
    if (num_elements_ <= num_threads_ * threshold_) {
      // to trigger a sane work sharing environment, we'll make sure
      // that each thread has at least "threshold_" elements to deal
      // with
      num_threads_ = 1;
      grain_ = num_elements_;
    }
  }

  inline size_t GetNumThreads() const noexcept { return num_threads_; }
  inline size_t GetGrain() const noexcept { return grain_; }
  inline void ResetSteps(void) { step_ = 0; }

  // this returns _inclusive_ ranges, so your loops must be from
  std::pair<size_t, size_t> Step() {
    const size_t from = step_ * grain_;
    size_t to = (step_ + 1) * grain_ - 1;

    if (to + threshold_ >= num_elements_)
      to = num_elements_ - 1;

    auto pair = std::make_pair(from, to);

    ++step_;

    return pair;
  }

private:
  size_t threshold_;
  size_t num_elements_;
  size_t num_threads_;
  size_t grain_;
  size_t step_;
};

int main(int argc, char *argv[]) {
  int opt = 0;
  std::uint64_t max_number = 0;

  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
    case 'n':
      {
        std::stringstream ss;
        ss << optarg;
        ss >> max_number;
      }
      break;
    default:
      std::cerr << "usage: " << std::endl;
      std::cerr << "  " << argv[0] << ": <-n> <upto-max>" << std::endl;
      break;
    }
  }

  std::cout << "generating data..." << std::endl;

  // you can use (n(first, last) / 2) for this, but the point of
  // this exercise is mostly to familiarize with concurrency in c++
  std::vector<std::uint64_t> numbers(max_number);
  {
    std::uint64_t counter = 1;
    std::generate(numbers.begin(), numbers.end(),
                  [&counter](){ return counter++; });
  }

  auto suggestion = Granulator(max_number);
  std::cout << "number of items in vector: " << numbers.size() << std::endl;
  std::cout << "number of cores: " << suggestion.GetNumThreads() << std::endl;
  std::cout << "grain size     : " << suggestion.GetGrain() << std::endl;

  MTVector<std::uint64_t> mt_vector;
  std::vector<std::thread> threads;

  auto work_fn = [](MTVector<std::uint64_t>& mtv,
                    const std::vector<std::uint64_t>& nms,
                    const std::pair<size_t,size_t>& rng) noexcept {
    std::uint64_t sum = 0;
    for (size_t i = rng.first; i <= rng.second; ++i) sum += nms[i];
    mtv.Add(sum);
  };

  for (size_t i = 0; i < suggestion.GetNumThreads(); ++i) {
    const auto range = suggestion.Step();

    threads.push_back(std::thread(work_fn,
                                  std::ref(mt_vector),
                                  std::ref(numbers),
                                  std::ref(range)));
  }

  for (auto& e : threads) e.join();

  std::uint64_t final_sum = 0;
  {
    auto parts = mt_vector.GetUnsafe();
    for (const auto& e : mt_vector.GetUnsafe())
      final_sum += e;
  }

  std::cout << "total sum: " << final_sum << std::endl;

  return 0;
}
