#pragma once

#include <string>
#include <vector>

using vi = std::vector<int>;

class CRubiksCubeBase
{
  public:
    CRubiksCubeBase(int n);

    void SetState(const vi &v)
    {
        currentState = v;
    };

    vi &GetState()
    {
        return currentState;
    };

    const vi &GetGoalState()
    {
        return goalState;
    };

    void DoMove(int move);

    virtual vi ApplyMove(int move, vi state);

    virtual void Show() = 0;

    virtual std::string Solve() = 0;

    virtual void Reset() = 0;

    virtual std::string GetRegex() = 0;

    virtual int GetMove(std::string s) = 0;

    virtual std::string Random(int n) = 0;

    virtual bool State2String(const vi &state, std::vector<std::string> &s) = 0;

    virtual bool String2State(const std::vector<std::string> &s, vi &state) = 0;

  public:
    vi currentState;
    vi goalState;
};
