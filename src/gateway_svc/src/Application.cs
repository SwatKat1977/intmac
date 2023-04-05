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
using shared.src;

namespace gateway_svc
{
    public class Application : BaseApplication
    {
        readonly private int MILLISECONDS_TO_SECONDS = 1000;

        private RedisClient sessionsClient;

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

            logger.Info("Opening REDIS sessions database...");

            if (!ConnectToSessionsRedis())
            {
                return false;
            }

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

        private bool ConnectToSessionsRedis()
        {
            bool isConnected = false;
            int redisConnectWait = 10;

            /* TEMPORARY */
            string redisHostname = "localhost";
            int redisPort = 6379;
            string redisPassword = "change-this-password";
            int redisRetries = 10;

            sessionsClient = new RedisClient(logger, redisHostname, redisPort,
                                             redisPassword);

            while (!isConnected && redisRetries != 0)
            {
                logger.Info("Attempting to connect to redis...");

                isConnected = sessionsClient.Initialise();

                if (!isConnected)
                {
                    logger.Error("Failed to connect to redis, retrying in" +
                                 $" {redisConnectWait} seconds...");
                    Thread.Sleep(redisConnectWait * MILLISECONDS_TO_SECONDS);
                    redisConnectWait *= 2;
                    redisRetries -= 1;
                }
            }

            if (!isConnected)
            {
                logger.Fatal("Failed to connect to sessions, aborting...");
                return false;
            }

            logger.Info("Connected to sessions redis database");
            return true;
        }
    };
}
