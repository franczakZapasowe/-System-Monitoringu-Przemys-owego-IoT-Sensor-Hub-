#pragma once
struct MachineState {
    int m_id;
    float m_temperature;
    bool m_flag;
};

struct ServerCommand {
    // true pracuj false zatrzymaj
    bool m_code;
};
