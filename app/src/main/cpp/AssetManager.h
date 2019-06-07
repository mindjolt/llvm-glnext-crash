//
// Created by Jesus Lopez on 4/24/19.
//

#ifndef TEST_BED_ASSETMANAGER_H
#define TEST_BED_ASSETMANAGER_H

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"

class AssetManager {
public:
    AssetManager(AAssetManager *manager) : manager(manager) {}

    std::string readAsset(const char *path);

private:
    AAssetManager *manager;
};

#pragma clang diagnostic pop

#endif //TEST_BED_ASSETMANAGER_H