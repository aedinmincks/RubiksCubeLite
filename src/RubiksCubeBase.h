#pragma once

#include <string>
#include <vector>

using vi = std::vector<int>;

class CRubiksCubeBase
{
  public:
    CRubiksCubeBase(int n);

    bool SetState(vi v);

    vi &GetState()
    {
        return currentState;
    };

    const vi &GetGoalState()
    {
        return goalState;
    };

    int GetSize()
    {
        return size;
    };

    void DoMove(int move);

    virtual vi ApplyMove(int move, vi state);

    virtual void Show() = 0;

    virtual std::string Solve() = 0;

    virtual void Reset() = 0;

    virtual std::string GetRegex() = 0;

    virtual int GetMove(std::string s) = 0;

  public:
    vi currentState;
    vi goalState;
    int size;
};
