#pragma once

class ResurrectionAPI
{
public:
    virtual const bool should_resurrect(RE::Actor*) { return false; };
    virtual void resurrect(RE::Actor*) {};
};
typedef void (*AddSubscriber_t)(std::unique_ptr<ResurrectionAPI> api);
