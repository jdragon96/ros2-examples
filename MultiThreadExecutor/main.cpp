#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <chrono>
#include <functional>
#include <memory>
#include <sstream>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node {
public:
  MinimalPublisher(std::string Name) : Node(Name), count_1(0), count_2(0) {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(
        500ms, std::bind(&MinimalPublisher::timer_callback, this));

    publisher_2 = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_2 = this->create_wall_timer(
        500ms, std::bind(&MinimalPublisher::timer_callback2, this));
    m_nodeName = Name;
  }

private:
  void timer_callback() {
    auto message = std_msgs::msg::String();

    std::stringstream ss;
    ss << "Hello, world! " + std::to_string(count_1++) + " / Thread ID : ";
    ss << std::this_thread::get_id();
    std::string mystring = ss.str();
    message.data = mystring;
    std::cout << mystring << std::endl;
    publisher_->publish(message);
  }
  void timer_callback2() {
    auto message = std_msgs::msg::String();
    std::stringstream ss;
    ss << "Hello, world! " + std::to_string(count_2++) + " / Thread ID : ";
    ss << std::this_thread::get_id();
    std::string mystring = ss.str();
    message.data = mystring;
    std::cout << mystring << std::endl;
    publisher_->publish(message);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_2;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_2;
  size_t count_1;
  size_t count_2;
  std::string m_nodeName;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  std::cout << "Thread pool size : " << executor.get_number_of_threads()
            << std::endl;
  auto node1 = std::make_shared<MinimalPublisher>("node_1");
  executor.add_node(node1);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}