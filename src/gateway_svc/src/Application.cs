/*
Copyright 2014-2023 Integrated Test Management Suite

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
using shared;

namespace gateway_svc
{
    public class Application : BaseApplication
    {
        public Application(log4net.ILog loggerInstance) : base(loggerInstance)
        {
        }

        protected override bool InitialiseApplication()
        {
            String buildDetails = $"V{VersionInformation.RELEASE_VERSION}-" +
                                  $"{VersionInformation.BUILD_VERSION}" +
                                  $"{VersionInformation.BUILD_TAG}";

            logger.Info($"ITEMS Gateway Service {buildDetails}");
            logger.Info(COPYRIGHT_TEXT);
            logger.Info(LICENSE_TEXT);

            return true;
        }

        // Method for main application.
        protected override void MainLoop()
        {
        }

        // Method for application shutdown.
        protected override void Shutdown()
        {
        }
    };
}
