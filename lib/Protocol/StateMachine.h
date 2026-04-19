#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

class IState {
public:
    virtual ~IState() = default;
    virtual void handle() = 0;
};

class StateMachine {
public:
    StateMachine();
    void setState(IState* state);
    void update();
private:
    IState* currentState;
};

#endif // STATE_MACHINE_H