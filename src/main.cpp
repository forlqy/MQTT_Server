#include <iostream>
#include <fstream>
#include <sstream>
#include <mosquitto.h>
#include <vector>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <yaml-cpp/yaml.h>
#include <json/json.h>
#include <iomanip>
#include <string>
#include <random>
#include <unordered_map>

const std::string SERIAL_DATA_TOPIC = "serial/data";
const std::string COMMAND_TOPIC = "command";
const std::string FEEDBACK_TOPIC = "feedback";



class Device {
public:
    std::string uuid;
    std::string key;
    std::string alias;
    int address;
    int startOffset;
    std::string deviceType;
    std::string description;
    std::vector<std::string> category;
    std::vector<std::string> fields;
    int acquisitionCycle;
    std::string modelType;
    std::string location;
    std::map<std::string, std::string> unit;
    std::string manufacturer;

    // 构造函数
    Device() {}

    Device(const std::string& uuid, const std::string& key, const std::string& alias, int address, int startOffset,
           const std::string& deviceType, const std::string& description, const std::vector<std::string>& category,
           const std::vector<std::string>& fields, int acquisitionCycle, const std::string& modelType,
           const std::string& location, const std::map<std::string, std::string>& unit,
           const std::string& manufacturer)
        : uuid(uuid),
          key(key),
          alias(alias),
          address(address),
          startOffset(startOffset),
          deviceType(deviceType),
          description(description),
          category(category),
          fields(fields),
          acquisitionCycle(acquisitionCycle),
          modelType(modelType),
          location(location),
          unit(unit),
          manufacturer(manufacturer) {}

    void setAcquisitionCycle(int newCycle){
        acquisitionCycle = newCycle;
    }
};

class DeviceManager {
private:
    std::unordered_map<std::string, Device> devices;

public:
    using ptr = std::shared_ptr<DeviceManager>;
    // 加载设备配置文件
    bool loadDeviceFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open device configuration file: " << filename << std::endl;
            return false;
        }

        Json::Value root;
        file >> root;

        Json::Value Devices = root["devices"];

        for(Json::Value& device : Devices){
            std::string uuid = device["uuid"].asString();
            
            Device newDevice(uuid, device["key"].asString(), device["alias"].asString(),
                      device["address"].asInt(), device["start-offset"].asInt(), device["device-type"].asString(),
                      device["description"].asString(), parseCategories(device["category"]),
                      parseFields(device["fields"]), device["acquisition-cycle"].asInt(),
                      device["model-type"].asString(), device["location"].asString(), parseUnit(device["unit"]),
                      device["manufacturer"].asString());

            devices[uuid] = newDevice;
        }



        file.close();

        return true;
    }

    // 获取设备列表
    std::unordered_map<std::string, Device> getDevices() const {
        return devices;
    }

    //
    Device getDeviceByUUID(const std::string& uuid){
        return devices[uuid];
    }

private:
    // 解析设备的分类
    std::vector<std::string> parseCategories(const Json::Value& categoriesJson) {
        std::vector<std::string> categories;
        for (const auto& category : categoriesJson) {
            categories.push_back(category.asString());
        }
        return categories;
    }

    // 解析设备的字段
    std::vector<std::string> parseFields(const Json::Value& fieldsJson) {
        std::vector<std::string> fields;
        for (const auto& field : fieldsJson) {
            fields.push_back(field.asString());
        }
        return fields;
    }

    // 解析设备的单位
    std::map<std::string, std::string> parseUnit(const Json::Value& unitJson) {
        std::map<std::string, std::string> unit;
        for (const auto& keyValue : unitJson.getMemberNames()) {
            unit[keyValue] = unitJson[keyValue].asString();
        }
        return unit;
    }
};

class DataSimulator {
public:
    using ptr = std::shared_ptr<DataSimulator>;
    std::map<std::string, std::string> simulateData(const Device& device) {
        std::map<std::string, std::string> simulatedData;

        if (device.deviceType == "sensor") {
            simulatedData = simulateHumidityData();
        }
        else if (device.deviceType == "control") {
            simulatedData = simulateLightData();
        }

        return simulatedData;
    }

private:
    std::random_device rd;
    std::mt19937 gen{rd()};

    std::map<std::string, std::string> simulateHumidityData() {
        std::uniform_real_distribution<> dis(0.0, 100.0);
        std::map<std::string, std::string> val;
        val.insert({"humidity", std::to_string(dis(gen))});
        val.insert({"temperature", std::to_string(dis(gen))});
        return val;
    }

    std::map<std::string, std::string> simulateLightData() {
        std::uniform_real_distribution<> dis(0.0, 1000.0);
        return {{"light", std::to_string(dis(gen))}};
    }
};



class DataAcquire {
private:
    DeviceManager::ptr deviceManager;
    DataSimulator::ptr dataSimulator;

public:
    using ptr = std::shared_ptr<DataAcquire>;

    DataAcquire() : deviceManager(), dataSimulator(){
        deviceManager = std::make_shared<DeviceManager>();
        dataSimulator = std::make_shared<DataSimulator>();
    }

    void acquire(const std::string& uuid, const std::map<std::string, std::string>& data) {
        Json::Value jsonData;
        jsonData["uuid"] = uuid; 
        
        for (const auto& kv : data) {
            jsonData[kv.first] = kv.second;
            
            std::cout << kv.first << ": " << kv.second << std::endl;
        }

        jsonData["timestamp"] = getCurrentTimestamp();
        acquireData(uuid, jsonData);
    }

    void acquireData(const std::string& uuid, const Json::Value& jsonData) {
            // Write the data to a file with the name of the device's UUID
            std::ofstream dataFile(uuid + ".txt", std::ios::out | std::ios::app);
            for(const auto& element : jsonData.getMemberNames()){
                dataFile << element << ": " << jsonData[element].asString() << std::endl;
                dataFile.flush();
            }
                
            dataFile.close();
        }
private:
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto itt = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(localtime(&itt), "%Y-%m-%dT%H:%M:%SZ");
        return ss.str();
    }
};

/*
class DataSender {
private:
    mqtt::async_client mqttClient;
    MessageHandler msgHandler;
public:
    using ptr = std::shared_ptr<DataSender>;
    DataSender(std::function<void()> callback) : mqttClient(MQTT_SERVER_HOST, "mos"), msgHandler(callback) {
        mqttClient.set_callback(msgHandler);

        mqtt::connect_options connOpts;
        connOpts.set_user_name("root");
        connOpts.set_password("root");
        mqttClient.connect(connOpts)->wait();
    }

    void send(const std::string& topic, const std::string& uuid, const std::map<std::string, std::string>& data) {
        Json::Value jsonData;
        jsonData["uuid"] = uuid; 
        for (const auto& kv : data) {
            jsonData[kv.first] = kv.second;
        }
        jsonData["timestamp"] = getCurrentTimestamp();

        Json::StreamWriterBuilder writer;
        std::string payload = Json::writeString(writer, jsonData);

        mqtt::message_ptr pubmsg = mqtt::make_message(topic, payload);
        mqttClient.publish(pubmsg)->wait_for(std::chrono::seconds(10));
    }

private:
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto itt = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(localtime(&itt), "%Y-%m-%dT%H:%M:%SZ");
        return ss.str();
    }
};
*/

class SerialManager{
private:
    std::vector<std::string> serialUUIDs;
    DeviceManager deviceManager;
    DataSimulator dataSimulator;
    DataAcquire::ptr dataAcquire;

public:
    using ptr =  std::shared_ptr<SerialManager>;
    SerialManager(){
        loadSerialConfig("serial_config.json");

        loadDevicesFromSerials();

        dataAcquire = std::make_shared<DataAcquire>();
    }

    // 加载设备配置文件
    bool loadSingleDeviceFromFile(const std::string& filename) {
        return deviceManager.loadDeviceFromFile(filename);
    }


    // 加载串口配置文件
    bool loadSerialConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open serial configuration file: " << filename << std::endl;
            return false;
        }

        Json::Value root;
        file >> root;

        const Json::Value& devicesJson = root["devices"];
        for (const auto& deviceJson : devicesJson) {
            std::string serialUUID = deviceJson["uuid"].asString();
            serialUUIDs.push_back(serialUUID);
        }

        file.close();

        return true;
    }

    // 
    bool loadDevicesFromSerials() {
        for (const auto& serialUUID : serialUUIDs) {
            std::string deviceConfigFilename = serialUUID + ".json";
            if (loadSingleDeviceFromFile(deviceConfigFilename)) {
                std::cout << "Loaded device: " << deviceConfigFilename << std::endl;
            }else {
                std::cerr << "Failed to load device: " << deviceConfigFilename << std::endl;
            }
        }
        return true;
    }


    
    // 模拟并发送设备数据
    void simulateAndSendDeviceData() {
        while (true) {
            for (const auto& uuid_device : deviceManager.getDevices()) {
                const auto& device = uuid_device.second;
                std::map<std::string, std::string> simulatedData = dataSimulator.simulateData(device);
                std::cout << device.uuid << std::endl;
               
                dataAcquire->acquire(device.uuid, simulatedData);
        
                // 按照设备的获取周期暂停
                std::this_thread::sleep_for(std::chrono::milliseconds(device.acquisitionCycle));
            }
        }
    }

    void updateDevicesAndSerialConfig(){
        for(const auto& serialUUID : serialUUIDs){
            std::string deviceConfigFilename = serialUUID + ".json";
            //reload
            std::ifstream file(deviceConfigFilename);
            if(!file.is_open()){
                std::cerr << "Failed to open device configuration file: " << deviceConfigFilename << std::endl;
                continue;
            }

            Json::Value root;
            file >> root;

            //get newCycle
            int newAcquisitionCycle = root["acquisition-cycle"].asInt();
            
            for(auto& uuid_device : deviceManager.getDevices()){
                auto& device = uuid_device.second;
                if(device.uuid == serialUUID){
                    device.setAcquisitionCycle(newAcquisitionCycle);
                    break;
                }
            }
            file.close();
        }
    }
};



class MQTTServer {
public:
    class UpdateConfig {
    public:
        UpdateConfig(){
            seri = std::make_shared<SerialManager>();
        }
        
        void update() {
            std::cout << "Updating configuration file..." << std::endl;

            seri->updateDevicesAndSerialConfig();
            std::cout << "Updating Success!" << std::endl;


        }
    private:
        SerialManager::ptr seri;
    };

    class FeedBack {
    private:
        struct mosquitto* mosqf;

    public:
        FeedBack() {
            mosquitto_lib_init();
            mosqf = mosquitto_new(nullptr, true, nullptr);
            if (!mosqf) {
                std::cerr << "Failed to create Mosquitto instance" << std::endl;
                return;
            }

            mosquitto_username_pw_set(mosqf, "root", "root");
        }

        ~FeedBack() {
            mosquitto_destroy(mosqf);
            mosquitto_lib_cleanup();
        }

        void send(const std::string& data) {
            std::cout << "Sending feedback: " << std::endl << data << std::endl;

            mosquitto_connect(mosqf, "10.0.0.18", 1883, 0);
            mosquitto_publish(mosqf, nullptr, FEEDBACK_TOPIC.c_str(), data.size(), data.c_str(), 0, false);
            mosquitto_disconnect(mosqf);

            std::cout << "Feedback published successfully." << std::endl;
        }
    };

private:
    struct mosquitto* mosq;
    UpdateConfig puc;
    SerialManager::ptr serialManager;
    DataAcquire::ptr dataAcquire;
public:
    MQTTServer() : mosq(nullptr), puc(), serialManager(), dataAcquire() {
        mosquitto_lib_init();
        mosq = mosquitto_new(nullptr, true, nullptr);
        if (!mosq) {
            std::cerr << "Failed to create Mosquitto instance" << std::endl;
            return;
        }

        mosquitto_username_pw_set(mosq, "root", "root");

        mosquitto_message_callback_set(mosq, message_callback);
        serialManager = std::make_shared<SerialManager>();
        dataAcquire = std::make_shared<DataAcquire>();
    }


    void start(const std::string& serverAddress, int serverPort) {
        int resultCode = mosquitto_connect(mosq, serverAddress.c_str(), serverPort, 60);
        if (resultCode != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "error calling mosquitto_connect\n");
            exit(1);
        }

        mosquitto_subscribe(mosq, nullptr, SERIAL_DATA_TOPIC.c_str(), 0);
        mosquitto_subscribe(mosq, nullptr, COMMAND_TOPIC.c_str(), 0);

        // Start the data acquisition thread
        std::thread acquireDataThread([&](){
            serialManager->simulateAndSendDeviceData();
        });
        acquireDataThread.detach();

        mosquitto_loop_forever(mosq, -1, 1);
    }

    
    private:
    static void message_callback(struct mosquitto* mosq, void* userdata, const struct mosquitto_message* message) {
        std::cout << "Message received" << std::endl;
        std::cout << "  - Topic: " << message->topic << std::endl;
        std::cout << "  - Payload: " << static_cast<const char*>(message->payload) << std::endl;

        MQTTServer* server = static_cast<MQTTServer*>(userdata);

        if (std::string(message->topic) == COMMAND_TOPIC) {
            server->handleCommand(static_cast<const char*>(message->payload));
        }
    }



    void handleCommand(const std::string& command) {
        UpdateConfig uconfig;
        
        FeedBack feedBack;

        if (command == "uc") {
            uconfig.update();
            puc = uconfig;
        } else if (command == "fb") {
            std::ifstream file("29C5F44E0A49470FB06367CDC9724FD3.txt");
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string fileContent = buffer.str();
            file.close();

            std::ifstream file1("B52F0A27BCE64509B51B723C35FEF877.txt");
            std::stringstream buffer1;
            buffer1 << file1.rdbuf();
            fileContent += buffer1.str();
            file1.close();

            feedBack.send(fileContent);
        }
    }
};

int main() {
    MQTTServer server;
    server.start("10.0.0.18", 1883);
    return 0;
}

