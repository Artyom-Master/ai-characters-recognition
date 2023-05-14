#include "CharactersSamples.h"
#include "HopfieldNetwork.h"

#include <iostream>
#include <chrono>

void checkSamples(const std::vector<int8_t>& character
	, const std::vector<std::vector<int8_t>>& noiseSamples)
{
	for (int i{ 0 }; i < noiseSamples.size(); ++i)
	{
		int diff{ 0 };

		for (int j{ 0 }; j < character.size(); ++j)
			if (character.at(j) != noiseSamples.at(i).at(j))
				++diff;

		std::cout << "Sample: " << i + 1 << "\nNoise: " << diff << "\n\n";
	}
}

void printCharacter(const std::vector<int8_t>& character)
{
	for (int i{ 0 }; i < character.size(); ++i)
	{
		switch (character.at(i))
		{
		case 1:
			std::cout << "- ";
			break;
		case -1:
			std::cout << "  ";
			break;
		}

		if ((i + 1) % 10 == 0)
			std::cout << '\n';
	}
}

void findCharacter(HopfieldNetwork& hopfieldNetwork
	, const std::vector<std::vector<int8_t>>& noiseSamples
	, long long& time)
{
	for (int i{ 0 }; i < noiseSamples.size(); ++i)
	{
		auto start{ std::chrono::high_resolution_clock::now() };

		auto result{ hopfieldNetwork.findPattern(noiseSamples.at(i), HopfieldNetworkSearchMode::sync) };

		auto finish{ std::chrono::high_resolution_clock::now() };
		auto timeSpan{ std::chrono::duration_cast<std::chrono::milliseconds>(finish - start) };

		std::cout << "\nNoise sample: " << i + 1 << '\n';
		std::cout << "Train time: " << timeSpan.count() << " ms\n";
		std::cout << "Found:\n";
		printCharacter(result);

		time += timeSpan.count();
	}
}

int main(int argc, char* argv[])
{
	//checkSamples(B_CHARACTER, B_NOISE_SAMPLES);
	//checkSamples(K_CHARACTER, K_NOISE_SAMPLES);
	//checkSamples(C_CHARACTER, C_NOISE_SAMPLES);

	HopfieldNetwork hopfieldNetwork;
	auto start{ std::chrono::high_resolution_clock::now() };

	hopfieldNetwork.train({ B_CHARACTER, K_CHARACTER, C_CHARACTER });

	auto finish{ std::chrono::high_resolution_clock::now() };
	auto timeSpan{ std::chrono::duration_cast<std::chrono::milliseconds>(finish - start) };
	std::cout << "Train time: " << timeSpan.count() << " ms\n";

	long long time{ 0 };

	std::cout << "\n\nPROCESS B LETTER SAMPLES:\n\n";
	findCharacter(hopfieldNetwork, B_NOISE_SAMPLES, time);
	std::cout << "\n\nPROCESS K LETTER SAMPLES:\n\n";
	findCharacter(hopfieldNetwork, K_NOISE_SAMPLES, time);
	std::cout << "\n\nPROCESS C LETTER SAMPLES:\n\n";
	findCharacter(hopfieldNetwork, C_NOISE_SAMPLES, time);

	std::cout << "\nAll data samples processing time: " << time << " ms\n";

	return 0;
}