#pragma once

#include "RubiksCubeBase.h"
#include "string"

class VCube3 : public CRubiksCubeBase
{
  public:
    VCube3();
    VCube3(const vi& state);

    virtual void Reset() override;

    virtual void Show() override;

    virtual vi ApplyMove(int move, vi state) override;

    virtual std::string Solve() override;

    virtual std::string GetRegex() override;

    virtual int GetMove(std::string s) override;

    virtual std::string Random(int n) override;

    virtual bool State2String(const vi &state, std::vector<std::string> &s) override;

    virtual bool String2State(const std::vector<std::string> &s, vi &state) override;

  private:
    static std::vector<std::string> goal;

    static vi id(vi state, int phase);

    static int inverse(int move);
};
