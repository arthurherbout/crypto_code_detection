#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

struct Walkway {
	Walkway() { }

	long double start;
	long double end;
	long double speed;

	bool operator<(const Walkway &other) const {
		return start < other.start;
	}
};

struct WalkwayComp {

	bool operator()(const Walkway &a, const Walkway &b) const {
		return a.speed < b.speed;
	}

};

int main(int arc, char **argv) {
	
	int numTestCases;
	std::cin >> numTestCases;
	for(int testCaseNum = 0; testCaseNum < numTestCases; testCaseNum++) {
		long double corridorLen, walkSpeed, runSpeed, runTime, numWalkways;
		std::cin >> corridorLen >> walkSpeed >> runSpeed >> runTime >> numWalkways;
		
#ifdef DEBUG
		std::cout << "Len: " << corridorLen << ", walkSpeed: " << walkSpeed << ", runSpeed: " << runSpeed << ", runTime: " << runTime << std::endl;
#endif

		std::vector<Walkway> walkways;
		for(int i = 0; i < numWalkways; i++) {
			Walkway w;
			std::cin >> w.start >> w.end >> w.speed;
			walkways.push_back(w);
		}

		std::sort(walkways.begin(), walkways.end());

		std::vector<Walkway> filledWalkways;
		int curPos = 0;
		for(std::vector<Walkway>::const_iterator walkwaysIt = walkways.begin();
		    walkwaysIt != walkways.end(); ++walkwaysIt) {
			
			if(walkwaysIt->start != curPos) {
				Walkway w;
				w.start = curPos;
				w.end = walkwaysIt->start;
				w.speed = 0;
				filledWalkways.push_back(w);
			}

			filledWalkways.push_back(*walkwaysIt);
			curPos = walkwaysIt->end;
		}
		
		if(curPos < corridorLen) {
			Walkway w;
			w.start = curPos;
			w.end = corridorLen;
			w.speed = 0;
			filledWalkways.push_back(w);
		}

		walkways = filledWalkways;

		std::sort(walkways.begin(), walkways.end(), WalkwayComp());

#ifdef DEBUG
		for(std::vector<Walkway>::const_iterator walkwaysIt = walkways.begin();
		    walkwaysIt != walkways.end(); ++walkwaysIt) {
			
			std::cout << walkwaysIt->start << " -> " << walkwaysIt->end << ": " << walkwaysIt->speed << std::endl;
		}

#endif
		
		if(runSpeed <= walkSpeed) {
			runTime = 0;
		}

		long double time = 0;
		long double totalRunTime = 0;
		for(std::vector<Walkway>::const_iterator walkwaysIt = walkways.begin();
		    walkwaysIt != walkways.end(); ++walkwaysIt) {
			
			if(walkwaysIt->end == walkwaysIt->start) {
				continue;
			}

			long double dist = walkwaysIt->end - walkwaysIt->start;

			long double curRunSpeed = runSpeed + walkwaysIt->speed;
			long double curWalkSpeed = walkSpeed + walkwaysIt->speed;

			long double timeIfRun = dist / curRunSpeed;
			long double maxRunTime = (double) runTime - totalRunTime;
			long double curRunTime = std::min(maxRunTime, timeIfRun);

			long double segmentTime = curRunTime + (dist - (curRunTime * curRunSpeed)) / curWalkSpeed;

#ifdef DEBUG
			std::cout << "Between " << walkwaysIt->start << " and " << walkwaysIt->end << " with dist " << dist << ", extra speed is " << walkwaysIt->speed << ", giving a run speed of " << curRunSpeed << " and a walk speed of " << curWalkSpeed << ".  My maximum run time is " << maxRunTime << ".  If I run the entire way, it will take " << timeIfRun << "s.  So the longest I can run is " << curRunTime << ".  Thus I run a distance of " << curRunTime * curRunSpeed << " and walk a distance of " << (dist - curRunTime * curRunSpeed) << ", for a total segment time of " << segmentTime << "." << std::endl;
#endif
			time += segmentTime;
			totalRunTime += curRunTime;
		}

		std::printf("Case #%d: %.9f\n", testCaseNum + 1, (double) time);
	}


	return 0;
}
