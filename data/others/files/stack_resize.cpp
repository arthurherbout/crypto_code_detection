#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <numeric>
#include <functional>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <thread>
#include <tuple>
#include <utility>
#include <limits>

using namespace std;

struct Solver {
	long long ans;
	void input() {
	}

	long long factorial(int n) {
		if (n <= 0) return 1;
		return n * factorial(n-1) % 1'000'000'009;
	}

	void solve() {
		ans = factorial(10'000'000);
	}

	void output(int testcase) {
		printf("Case #%d: %d\n", testcase, (int)ans);
	}
};

void *starter(void *data) {
	Solver *solver = static_cast<Solver *>(data);
	solver->solve();
	return nullptr;
}

int main() {
	int testCaseCount;
	scanf("%d", &testCaseCount);

	pthread_attr_t thread_attr;
	{
		if (pthread_attr_init(&thread_attr) != 0) {
			exit(1);
		}
		pthread_attr_setstacksize(&thread_attr, 160 * 1048576);
	}

	const int batchSize = max(1, (int)(thread::hardware_concurrency() / 2));
	for (int testcase = 1; testcase <= testCaseCount; testcase += batchSize) {
		vector<unique_ptr<Solver>> solvers; solvers.reserve(batchSize);
		vector<pthread_t> threads; threads.reserve(batchSize);
		for (int batchInd = 0; batchInd < batchSize && testcase + batchInd <= testCaseCount; batchInd++) {
			solvers.emplace_back(new Solver());
		}
		for (auto &solver : solvers) {
			solver->input();
			{
				pthread_t thread_id;
				if (pthread_create(&thread_id, &thread_attr, &starter, static_cast<void *>(solver.get())) != 0) {
					exit(1);
				}
				threads.emplace_back(thread_id);
			}
		}
		for (int batchInd = 0; batchInd < (int)solvers.size(); batchInd++) {
			void *retval;
			if (pthread_join(threads[batchInd], &retval) != 0) {
				exit(1);
			}
			solvers[batchInd]->output(testcase + batchInd);
		}
	}
	return 0;
}

