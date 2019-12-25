// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_RpcLibClientBase_hpp
#define air_RpcLibClientBase_hpp

#include "common/Common.hpp"
#include "common/CommonStructs.hpp"
#include "common/ImageCaptureBase.hpp"
#include "physics/Kinematics.hpp"
#include "physics/Environment.hpp"

#include <map>

namespace msr { namespace airlib {

//common methods for RCP clients of different vehicles
class RpcLibClientBase {
public:
    enum class ConnectionState : uint {
        Initial = 0, Connected, Disconnected, Reset, Unknown
    };
public:
    RpcLibClientBase(const string& ip_address = "localhost", uint16_t port = 41451, float timeout_sec = 60);
    virtual ~RpcLibClientBase();    //required for pimpl

    void confirmConnection();
    void reset();

    ConnectionState getConnectionState();
    bool ping();
    int getClientVersion() const;
    int getServerVersion() const;
    int getMinRequiredServerVersion() const;
    int getMinRequiredClientVersion() const;

    bool simIsPaused() const;
    void simPause(bool is_paused);
    void simContinueForTime(double seconds);

    Pose simGetObjectPose(const std::string& object_name) const;
    bool simSetObjectPose(const std::string& object_name, const Pose& pose, bool teleport = true);
    bool simSpawnStaticMeshObject(const std::string& object_class_name, const std::string& object_class, const Pose& pose);
    bool simDeleteObject(const std::string& object_name);
    
    //task management APIs
    void cancelLastTask(const std::string& vehicle_name = "");
    virtual RpcLibClientBase* waitOnLastTask(bool* task_result = nullptr, float timeout_sec = Utils::nan<float>());

    bool simSetSegmentationObjectID(const std::string& mesh_name, int object_id, bool is_name_regex = false);
    int simGetSegmentationObjectID(const std::string& mesh_name) const;
    void simPrintLogMessage(const std::string& message, std::string message_param = "", unsigned char severity = 0);

    std::map<std::string, std::map<std::string, double> > readSensors(const std::string vehicle_name = "");
    msr::airlib::RayCastResponse simRayCast(const msr::airlib::RayCastRequest request, const std::string vehicle_name = "");
    void simSetDrawableShapes(const msr::airlib::DrawableShapeRequest request, const std::string vehicle_name = "");

    void addDrawableShapePoint(msr::airlib::DrawableShapeRequest &request, const std::string &shape_name, const std::string &reference_frame_link, float x, float y, float z, float size, int color_r, int color_g, int color_b, int color_a);
    void addDrawableShapeSphere(msr::airlib::DrawableShapeRequest &request, const std::string &shape_name, const std::string &reference_frame_link, float x, float y, float z, float radius, float thickness, int number_of_segments, int color_r, int color_g, int color_b, int color_a);
    void addDrawableShapeCircle(msr::airlib::DrawableShapeRequest &request, const std::string &shape_name, const std::string &reference_frame_link, float x, float y, float z, float normal_x, float normal_y, float normal_z, float radius, float thickness, int number_of_segments, int color_r, int color_g, int color_b, int color_a);
    void addDrawableShapeBox(msr::airlib::DrawableShapeRequest &request, const std::string &shape_name, const std::string &reference_frame_link, float x, float y, float z, float extents_x, float extents_y, float extents_z, float thickness, int color_r, int color_g, int color_b, int color_a);
    void addDrawableShapeLine(msr::airlib::DrawableShapeRequest &request, const std::string &shape_name, const std::string &reference_frame_link, float start_x, float start_y, float start_z, float end_x, float end_y, float end_z, float thickness, int color_r, int color_g, int color_b, int color_a);
    

    bool armDisarm(bool arm, const std::string& vehicle_name = "");
    bool isApiControlEnabled(const std::string& vehicle_name = "") const;
    void enableApiControl(bool is_enabled, const std::string& vehicle_name = "");

    msr::airlib::GeoPoint getHomeGeoPoint(const std::string& vehicle_name = "") const;

    msr::airlib::LidarData getLidarData(const std::string& lidar_name = "", const std::string& vehicle_name = "") const;

    Pose simGetVehiclePose(const std::string& vehicle_name = "") const;
    void simSetVehiclePose(const Pose& pose, bool ignore_collision, const std::string& vehicle_name = "");

    vector<msr::airlib::GeoPoint> xyzToGeoPoints(const vector<msr::airlib::Vector3r> &xyz_points, const std::string& vehicle_name = "");

    vector<ImageCaptureBase::ImageResponse> simGetImages(vector<ImageCaptureBase::ImageRequest> request, const std::string& vehicle_name = "");
    vector<uint8_t> simGetImage(const std::string& camera_name, ImageCaptureBase::ImageType type, const std::string& vehicle_name = "");
    void simSetCameraPose(const CameraPose camera_pose, const std::string& vehicle_name = "");

    CollisionInfo simGetCollisionInfo(const std::string& vehicle_name = "") const;

    CameraInfo simGetCameraInfo(const std::string& camera_name, const std::string& vehicle_name = "") const;
    void simSetCameraOrientation(const std::string& camera_name, const Quaternionr& orientation, const std::string& vehicle_name = "");

    msr::airlib::Kinematics::State simGetGroundTruthKinematics(const std::string& vehicle_name = "") const;
    msr::airlib::Environment::State simGetGroundTruthEnvironment(const std::string& vehicle_name = "") const;



    //----------- APIs to control ACharacter in scene ----------/
    void simCharSetFaceExpression(const std::string& expression_name, float value, const std::string& character_name = "");
    float simCharGetFaceExpression(const std::string& expression_name, const std::string& character_name = "") const;
    std::vector<std::string> simCharGetAvailableFaceExpressions();
    void simCharSetSkinDarkness(float value, const std::string& character_name = "");
    float simCharGetSkinDarkness(const std::string& character_name = "") const;
    void simCharSetSkinAgeing(float value, const std::string& character_name = "");
    float simCharGetSkinAgeing(const std::string& character_name = "") const;
    void simCharSetHeadRotation(const msr::airlib::Quaternionr& q, const std::string& character_name = "");
    msr::airlib::Quaternionr simCharGetHeadRotation(const std::string& character_name = "") const;
    void simCharSetBonePose(const std::string& bone_name, const msr::airlib::Pose& pose, const std::string& character_name = "");
    msr::airlib::Pose simCharGetBonePose(const std::string& bone_name, const std::string& character_name = "") const;
    void simCharResetBonePose(const std::string& bone_name, const std::string& character_name = "");
    void simCharSetFacePreset(const std::string& preset_name, float value, const std::string& character_name = "");
    void simSetFacePresets(const std::unordered_map<std::string, float>& presets, const std::string& character_name = "");
    void simSetBonePoses(const std::unordered_map<std::string, msr::airlib::Pose>& poses, const std::string& character_name = "");
    std::unordered_map<std::string, msr::airlib::Pose> simGetBonePoses(const std::vector<std::string>& bone_names, const std::string& character_name = "") const;

protected:
    void* getClient();
    const void* getClient() const;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

}} //namespace
#endif
