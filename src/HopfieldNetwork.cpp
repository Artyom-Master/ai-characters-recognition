#include "HopfieldNetwork.h"

#include <iostream>

HopfieldNetwork::HopfieldNetwork(size_t neuronsCount)
	: m_weights(neuronsCount * neuronsCount)
	, m_neurons(neuronsCount)
	, m_trainPatterns()
{

}

void HopfieldNetwork::train(const std::vector<std::vector<int8_t>>& trainPatterns)
{
	auto patternsCount{ trainPatterns.size() };
	m_trainPatterns = trainPatterns;

	for (const auto& pattern : trainPatterns)
	{
		for (size_t i{ 0 }; i < pattern.size(); ++i)
		{
			for (size_t j{ 0 }; j < pattern.size(); ++j)
			{
				if (i == j)
					m_weights[std::to_string(i) + std::to_string(j)] = 0;
				else
					m_weights[std::to_string(i) + std::to_string(j)] += pattern.at(i) * pattern.at(j);
			}
		}
	}

	for (auto& it : m_weights)
		it.second /= patternsCount;
}

void HopfieldNetwork::setNeuronsForSearch(const std::vector<int8_t>& character
	, HopfieldNetworkSearchMode mode)
{
	for (size_t i{ 0 }; i < m_neurons.size(); ++i)
		m_neurons[i] = character.at(i);

	bool hasChanges{ true };
	double currentSum{ 0.0 };
	int8_t newNeuronValue{ 1 };
	auto newNeuronsState{ m_neurons };

	while (hasChanges)
	{
		hasChanges = false;

		for (size_t i{ 0 }; i < character.size(); ++i)
		{
			currentSum = 0.0;

			for (size_t j{ 0 }; j < character.size(); ++j)
				currentSum += m_weights.at(std::to_string(i) + std::to_string(j)) * m_neurons.at(j);

			if (currentSum > ACTIVATION_THREASHOLD)
				newNeuronValue = 1;
			else
				newNeuronValue = -1;

			if (m_neurons.at(i) != newNeuronValue)
				hasChanges = true;

			switch (mode)
			{
			case HopfieldNetworkSearchMode::async:
				m_neurons[i] = newNeuronValue;
				break;
			case HopfieldNetworkSearchMode::sync:
				newNeuronsState[i] = newNeuronValue;
				break;
			}
		}

		if (mode == HopfieldNetworkSearchMode::sync)
			m_neurons = newNeuronsState;
	}

	//for (int i{ 0 }; i < m_neurons.size(); ++i)
	//{
	//	switch (m_neurons.at(i))
	//	{
	//	case 1:
	//		std::cout << "- ";
	//		break;
	//	case -1:
	//		std::cout << "  ";
	//		break;
	//	}

	//	if ((i + 1) % 10 == 0)
	//		std::cout << '\n';
	//}

	//std::cout << '\n';
}

std::vector<int8_t> HopfieldNetwork::findPattern(const std::vector<int8_t>& character
	, HopfieldNetworkSearchMode mode)
{
	setNeuronsForSearch(character, mode);

	size_t diff{ 0 };
	int minIndex{};
	size_t minDiff{ m_neurons.size() + 1 };

	for (int i{ 0 }; i < m_trainPatterns.size(); ++i)
	{
		for (int j{ 0 }; j < m_trainPatterns.at(i).size(); ++j)
			diff += m_neurons.at(j) == m_trainPatterns.at(i).at(j) ? 0 : 1;

		if (diff < minDiff)
		{
			minDiff = diff;
			minIndex = i;
		}

		diff = 0;
	}

	return m_trainPatterns.at(minIndex);
}