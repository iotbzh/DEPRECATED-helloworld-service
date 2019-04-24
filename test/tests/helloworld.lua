--[[
    Copyright (C) 2018 "IoT.bzh"
    Author Romain Forlot <romain.forlot@iot.bzh>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.


    NOTE: strict mode: every global variables should be prefixed by '_'
--]]

function _callback(responseJ)
  _AFT.assertStrContains(responseJ.request.status, "success")
end

function _callbackError(responseJ)
  _AFT.assertStrContains(responseJ.request.info, "verb pingfail unknown within api helloworld")
end

_AFT.testVerbStatusSuccess('testPingSuccess','helloworld', 'ping', {})
_AFT.setBefore("testPingSuccess",function() print("~~~~~ Begin testPingSuccess ~~~~~") end)
_AFT.setAfter("testPingSuccess",function() print("~~~~~ End testPingSuccess ~~~~~") end)

_AFT.testVerbResponseEquals('testPingSuccessAndResponse','helloworld', 'ping', {}, "Ping count = %d")
_AFT.testVerbCb('testPingSuccessCallback','helloworld', 'ping', {}, _callback)

_AFT.testVerbStatusError('testPingError', 'helloworld', 'pingfail', {})
_AFT.testVerbResponseEqualsError('testPingErrorAndResponse', 'helloworld', 'pingfail', {}, "Ping Binder Daemon succeeds")
_AFT.testVerbCbError('testPingErrorCallback', 'helloworld', 'pingfail', {}, _callbackError)