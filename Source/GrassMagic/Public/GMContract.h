#pragma once

#include "CoreMinimal.h"

#include <functional>

#define CONTRACT GMContract DesignContract; DesignContract
#define BOOL_COND(bool_cond) [&] { return bool_cond; }


class GMContract
{
public:

	using ConditionCheckType = std::function<bool()>;


	GMContract() = default;
	GMContract(const GMContract&) = delete;
	GMContract(GMContract&&) = delete;

	GMContract& operator=(const GMContract&) = delete;
	GMContract& operator=(GMContract&&) = delete;

	GMContract& Precondition(ConditionCheckType Pred) 
	{
		check(Pred());

		return *this;
	};
	
	void Postcondition(ConditionCheckType Post)
	{
		PostconditionFunc = Post;
	}

	~GMContract()
	{
		if(PostconditionFunc) 
			check(PostconditionFunc());
	}

private:

	ConditionCheckType PostconditionFunc;
};