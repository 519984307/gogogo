#ifndef ENGINECALLBACK_H
#define ENGINECALLBACK_H

class EngineCallback {
public:
    virtual int OnEngineCallback(int event, int type, void *data) = 0;
};

#endif // ENGINECALLBACK_H
