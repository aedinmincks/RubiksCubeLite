#pragma once

#include <vector>

using vi = std::vector<int>;

class CRubiksCubeBase
{
  public:
    CRubiksCubeBase(int n);

    bool SetState(vi v);

    vi &GetState()
    {
        return state;
    };

    const vi &GetGoalState()
    {
        return goalstate;
    };

    int GetSize()
    {
        return size;
    };

    void DoMove(int move);

    virtual vi ApplyMove(int move, vi v);

    virtual void Show() = 0;

    virtual void Solve() = 0;

  public:
    vi state;
    vi goalstate;
    int size;
};
