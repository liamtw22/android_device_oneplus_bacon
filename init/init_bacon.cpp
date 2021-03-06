/*
   Copyright (c) 2016, The CyanogenMod Project
                 2020, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/file.h>
#include <android-base/strings.h>

#include "DeviceLibinit.h"
#include "init_msm8974.h"
#include "vendor_init.h"

using android::base::ReadFileToString;
using android::base::Split;

void import_kernel_cmdline(const std::function<void(const std::string&, const std::string&)>& fn) {
    std::string cmdline;
    android::base::ReadFileToString("/proc/cmdline", &cmdline);

    for (const auto& entry : android::base::Split(android::base::Trim(cmdline), " ")) {
        std::vector<std::string> pieces = android::base::Split(entry, "=");
        if (pieces.size() == 2) {
            fn(pieces[0], pieces[1]);
        }
    }
}

static void import_kernel_nv(const std::string& key, const std::string& value)
{
    if (key.empty()) return;

    if (key == "oppo.rf_version") {
        property_override("ro.oppo.rf_version", value.c_str());
    } else if (key == "oppo.pcb_version") {
        property_override("ro.oppo.pcb_version", value.c_str());
    }
}

static void vendor_set_sku(const std::string& key, const std::string& value)
{
    if (key.empty()) return;

    if (key == "mdss_mdp.panel" && value == "1:dsi:0:qcom,mdss_dsi_jdi_1080p_cmd") {
        property_override("ro.boot.hardware.sku", "jdi");
    } else if (key == "mdss_mdp.panel" && value == "1:dsi:0:qcom,mdss_dsi_sharp_1080p_cmd") {
        property_override("ro.boot.hardware.sku", "sharp");
    } else if (key == "mdss_mdp.panel" && value == "1:dsi:0:qcom,mdss_dsi_truly_1080p_cmd") {
        property_override("ro.boot.hardware.sku", "truly");
    }
}

void vendor_load_device_properties()
{
    import_kernel_cmdline(import_kernel_nv);
    import_kernel_cmdline(vendor_set_sku);
}
