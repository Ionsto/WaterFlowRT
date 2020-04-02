#pragma once
#include <iostream>
template<class t, int max>
class SwapList {
public:
	static constexpr int MaxSlots = max;
	std::array<std::unique_ptr<t>, MaxSlots> ParticleList;
	std::array<int, MaxSlots> Ids;
	std::array<int, MaxSlots> ReverseIds;
	//Stable ids
	int ParticleCount = 0;
	SwapList() {
		Ids.fill(-1);
	}
	t& GetParticle(int i)
	{
		return *ParticleList[Ids[i]].get();
	}
	std::array<std::unique_ptr<t>, MaxSlots>& RawData()
	{
		return ParticleList;
	}
	int FindFreeId() {
		for (int i = 0; i < MaxSlots; ++i)
		{
			if (Ids[i] == -1)
			{
				return i;
			}
		}
		return -1;
	}
	int AddParticle(std::unique_ptr<t> && add)
	{
		if (ParticleCount != MaxSlots)
		{
			//id is an id slot
			int id = FindFreeId();
			if (id != -1)
			{
				//Ids points to particle location
				Ids[id] = ParticleCount;
				ReverseIds[ParticleCount] = id;
				std::swap(ParticleList[ParticleCount++], add);
			}
			return id;
		}
		std::cout << "Failed to add particle\n";
		return -1;
	}
	void RemoveParticle(int position)
	{
		if (position == ParticleCount - 1)
		{

			int id = ReverseIds[position];
			Ids[id] = -1;
			ReverseIds[id] = -1;
			ParticleCount--;
			return;
		}
		if (position < ParticleCount)
		{
			int idend = ReverseIds[ParticleCount - 1];
			int iddelete = ReverseIds[position];
			Ids[idend] = position;
			ReverseIds[position] = idend;
			Ids[iddelete] = -1;
			ReverseIds[ParticleCount-1] = -1;
			std::swap(ParticleList[position],ParticleList[ParticleCount-- - 1]);
		}
		else {
			throw;
			//wtf
		}
	}
};