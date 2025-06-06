//
// Copyright (C) 2023 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <chrono>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include <android-base/logging.h>

#include "cuttlefish/common/libs/utils/result.h"
#include "cuttlefish/host/commands/cvd/cache/cache.h"
#include "cuttlefish/host/libs/web/android_build_api.h"
#include "cuttlefish/host/libs/web/android_build_string.h"
#include "cuttlefish/host/libs/web/cas/cas_downloader.h"
#include "cuttlefish/host/libs/web/chrome_os_build_string.h"

namespace cuttlefish {

inline constexpr bool kDefaultUseGceMetadata = false;
inline constexpr char kDefaultCredentialFilepath[] = "";
inline constexpr char kDefaultServiceAccountFilepath[] = "";
inline constexpr char kDefaultApiKey[] = "";
inline constexpr char kDefaultCredentialSource[] = "";
inline constexpr char kDefaultProjectID[] = "";
inline constexpr std::chrono::seconds kDefaultWaitRetryPeriod =
    std::chrono::seconds(20);
inline constexpr bool kDefaultExternalDnsResolver =
#ifdef __BIONIC__
    true;
#else
    false;
#endif
inline constexpr bool kDefaultEnableCaching = true;
inline constexpr char kDefaultBuildString[] = "";
inline constexpr bool kDefaultDownloadImgZip = true;
inline constexpr bool kDefaultDownloadTargetFilesZip = false;
inline constexpr char kDefaultTargetDirectory[] = "";
inline constexpr bool kDefaultKeepDownloadedArchives = false;

inline constexpr char kDefaultBuildTarget[] =
    "aosp_cf_x86_64_only_phone-userdebug";

struct CredentialFlags {
  bool use_gce_metadata = kDefaultUseGceMetadata;
  std::string credential_filepath = kDefaultCredentialFilepath;
  std::string service_account_filepath = kDefaultServiceAccountFilepath;
};

struct BuildApiFlags {
  std::string api_key = kDefaultApiKey;
  CredentialFlags credential_flags;
  std::string credential_source = kDefaultCredentialSource;
  std::string project_id = kDefaultProjectID;
  std::chrono::seconds wait_retry_period = kDefaultWaitRetryPeriod;
  bool external_dns_resolver = kDefaultExternalDnsResolver;
  std::string api_base_url = kAndroidBuildServiceUrl;
  bool enable_caching = kDefaultEnableCaching;
  std::size_t max_cache_size_gb = kDefaultCacheSizeGb;
  CasDownloaderFlags cas_downloader_flags;
};

struct VectorFlags {
  std::vector<std::optional<BuildString>> default_build;
  std::vector<std::optional<BuildString>> system_build;
  std::vector<std::optional<BuildString>> kernel_build;
  std::vector<std::optional<BuildString>> boot_build;
  std::vector<std::optional<BuildString>> bootloader_build;
  std::vector<std::optional<BuildString>> android_efi_loader_build;
  std::vector<std::optional<BuildString>> otatools_build;
  std::vector<std::optional<ChromeOsBuildString>> chrome_os_build;
  std::vector<bool> download_img_zip;
  std::vector<bool> download_target_files_zip;
  std::vector<std::string> boot_artifact;
};

struct FetchFlags {
  std::string target_directory = kDefaultTargetDirectory;
  std::vector<std::string> target_subdirectory;
  std::optional<BuildString> host_package_build;
  bool keep_downloaded_archives = kDefaultKeepDownloadedArchives;
  android::base::LogSeverity verbosity = android::base::INFO;
  bool helpxml = false;
  BuildApiFlags build_api_flags;
  VectorFlags vector_flags;
  int number_of_builds = 0;
  std::vector<std::string> host_substitutions;

  static Result<FetchFlags> Parse(std::vector<std::string>& args);
};

}  // namespace cuttlefish
