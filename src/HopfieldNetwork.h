#pragma once

#include <vector>
#include <unordered_map>
#include <string>

enum class HopfieldNetworkSearchMode
{
	async,
	sync
};

class HopfieldNetwork
{
private:
	std::unordered_map<std::string, double> m_weights;
	std::vector<int8_t> m_neurons;
	std::vector<std::vector<int8_t>> m_trainPatterns;

	static constexpr size_t NEURONS_COUNT{ 100 };
	static constexpr double ACTIVATION_THREASHOLD{ 0.0 };

public:
	HopfieldNetwork(size_t neuronsCount = NEURONS_COUNT);

	void train(const std::vector<std::vector<int8_t>>& trainPatterns);
	std::vector<int8_t> findPattern(const std::vector<int8_t>& character
		, HopfieldNetworkSearchMode mode = HopfieldNetworkSearchMode::async);

private:
	void setNeuronsForSearch(const std::vector<int8_t>& character
		, HopfieldNetworkSearchMode mode);
};