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

namespace gateway_svc
{
    class GatewayService
    {
        static void Main(string[] args)
        {
            var currentMethod = System.Reflection.MethodBase.GetCurrentMethod();

            if (currentMethod == null)
            {
                Console.WriteLine("[ERROR] Unable to initialise logging system!");
                return;
            }

            log4net.ILog logger = log4net.LogManager.GetLogger(
                currentMethod.DeclaringType);
            log4net.Config.XmlConfigurator.Configure();

            //log.Info("Test Log4Net info");

            Application application = new Application(logger);

            if (!application.Initialise())
            {
                return;
            }

            application.Run();
        }
    }

}