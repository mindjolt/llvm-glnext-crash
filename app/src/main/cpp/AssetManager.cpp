//
// Created by Jesus Lopez on 4/24/19.
//

#include <android/asset_manager.h>
#include <string>

#include "AssetManager.h"

std::string AssetManager::readAsset(const char *path) {
    auto asset = AAssetManager_open(manager, path, AASSET_MODE_BUFFER);
    auto size = static_cast<unsigned int>(AAsset_getLength(asset));
    std::string rv;
    rv.resize(size);
    AAsset_read(asset, const_cast<char *>(rv.data()), size);
    return rv;
}
