/*
Jabra Browser Integration
https://github.com/gnaudio/jabra-browser-integration

MIT License

Copyright (c) 2017 GN Audio A/S (Jabra)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <climits>
#include "stdafx.h"
#include "CmdOffHook.h"

CmdOffHook::CmdOffHook(HeadsetIntegrationService* headsetIntegrationService)
{
  m_headsetIntegrationService = headsetIntegrationService;
}

CmdOffHook::~CmdOffHook()
{
}

bool CmdOffHook::CanExecute(std::string cmd)
{
  return (cmd == "offhook");
}

void CmdOffHook::Execute(std::string cmd)
{
  unsigned deviceId = m_headsetIntegrationService->GetCurrentDeviceId();
  if (deviceId == USHRT_MAX)
  {
    m_headsetIntegrationService->Error("No device");
    return;
  }

  Jabra_GetLock(deviceId);

  // Stop ringer
  if (m_headsetIntegrationService->GetRingerStatus(deviceId))
  {
    Jabra_SetRinger(deviceId, false);
    m_headsetIntegrationService->SetRingerStatus(deviceId, false);
  }

  Jabra_ReturnCode ret = Jabra_SetOffHook(deviceId, true);
  if (ret != Return_Ok)
  {
    m_headsetIntegrationService->Error("Unable to offhook");
    return;
  }

  m_headsetIntegrationService->SetHookStatus(deviceId, true);
}
