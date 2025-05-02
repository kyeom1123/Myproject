#pragma once

#include <vsomeip/vsomeip.hpp>
#include <memory>
#include <string>

/**
 * @class speed_subscriber
 * @brief A class that subscribes to vehicle speed events using vsomeip
 */
class speed_subscriber {
public:
    /**
     * @brief Constructor initializes the vsomeip application and sets up event handlers
     * @throws std::runtime_error if initialization fails
     */
    speed_subscriber();
    
    /**
     * @brief Destructor ensures proper cleanup of resources
     */
    ~speed_subscriber();

    /**
     * @brief Starts the vsomeip application
     * @throws std::runtime_error if start fails
     */
    void start();

private:
    /**
     * @brief Handles incoming speed events
     * @param msg The received message containing speed data
     */
    void on_event(const std::shared_ptr<vsomeip::message> &msg);

    /**
     * @brief Loads configuration from file
     * @return true if configuration was loaded successfully
     */
    bool load_configuration();

    std::shared_ptr<vsomeip::application> app_;
    
    // Configuration parameters
    vsomeip::service_t SERVICE_ID;
    vsomeip::instance_t INSTANCE_ID;
    vsomeip::eventgroup_t EVENTGROUP_ID;
    vsomeip::event_t EVENT_ID;
    
    // Application name
    const std::string APP_NAME = "Client";
}; 