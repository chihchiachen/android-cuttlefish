/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "cuttlefish/host/libs/config/fastboot/fastboot.h"

#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <fruit/component.h>
#include <fruit/fruit_forward_decls.h>
#include <fruit/macro.h>

#include "cuttlefish/common/libs/utils/flag_parser.h"
#include "cuttlefish/common/libs/utils/result.h"
#include "cuttlefish/host/libs/config/config_flag.h"
#include "cuttlefish/host/libs/config/feature.h"

namespace cuttlefish {
namespace {

class FastbootConfigFlagImpl : public FastbootConfigFlag {
 public:
  INJECT(FastbootConfigFlagImpl(FastbootConfig& config, ConfigFlag& config_flag))
      : config_(config), config_flag_(config_flag) {}

  std::string Name() const override { return "FastbootConfigFlagImpl"; }

  std::unordered_set<FlagFeature*> Dependencies() const override {
    return {static_cast<FlagFeature*>(&config_flag_)};
  }

  Result<void> Process(std::vector<std::string>& args) override {
    bool proxy_fastboot = true;
    Flag proxy_fastboot_flag = GflagsCompatFlag(kName, proxy_fastboot);
    CF_EXPECT(ConsumeFlags({proxy_fastboot_flag}, args),
              "Failed to parse proxy_fastboot config flags");
    config_.SetProxyFastboot(proxy_fastboot);
    return {};
  }

  bool WriteGflagsCompatHelpXml(std::ostream& out) const override {
    bool proxy_fastboot = config_.ProxyFastboot();
    Flag proxy_fastboot_flag = GflagsCompatFlag(kName, proxy_fastboot).Help(kHelp);
    return WriteGflagsCompatXml({proxy_fastboot_flag}, out);
  }

 private:
  static constexpr char kName[] = "proxy_fastboot";
  static constexpr char kHelp[] = "Enstablish fastboot TCP proxy";

  FastbootConfig& config_;
  ConfigFlag& config_flag_;
};

}  // namespace

fruit::Component<fruit::Required<FastbootConfig, ConfigFlag>, FastbootConfigFlag>
FastbootConfigFlagComponent() {
  return fruit::createComponent()
      .bind<FastbootConfigFlag, FastbootConfigFlagImpl>()
      .addMultibinding<FlagFeature, FastbootConfigFlag>();
}

}  // namespace cuttlefish
