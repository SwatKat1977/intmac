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

namespace shared
{
    public abstract class BaseApplication
    {
        protected const String COPYRIGHT_TEXT = "Copyright 2014-2023 Integrated Test Management Suite";
        protected const String LICENSE_TEXT = "Licensed under the Apache License, Version 2.0";

        protected log4net.ILog logger;
        protected bool isInitialised;
        protected bool shutdownRequested;

        public BaseApplication(log4net.ILog logger)
        {
            this.logger = logger;
            this.isInitialised = false;
            this.shutdownRequested = false;
        }

        public bool Initialise()
        {

            bool initStatus = false;

            if (InitialiseApplication())
            {
                isInitialised = true;
                initStatus = true;
            }

            return initStatus;
        }

        public void Run()
        {
            while (!shutdownRequested && isInitialised)
            {
                MainLoop();
                Thread.Sleep(500);
            }

            logger.Info("Exiting application entrypoint...");
        }

        /*
        Stop the application, it will wait until shutdown has been marked as
        completed before calling the shutdown method.
        */
        public void Stop()
        {
            logger.Info("Stopping application...");
            logger.Info("Waiting for application shutdown to complete");

            shutdownRequested = true;

            Shutdown();
        }

        // Application initialisation.It should return a boolean
        // true => Successful, false => Unsuccessful.
        protected abstract bool InitialiseApplication();

        // Abstract method for main application.
        protected abstract void MainLoop();

        // Abstract method for application shutdown.
        protected abstract void Shutdown();
    }
}
