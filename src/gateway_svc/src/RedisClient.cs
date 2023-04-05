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
using Newtonsoft.Json;
using StackExchange.Redis;

namespace gateway_svc
{
    /* Enumeration for account authentication type */
    public enum AuthenticationType
    {
        GoogleApi,
        Facebook,
        Manual
    }

    public class SessionData
    {
        public string Email { get; set; }
        public string AuthenticationType { get; set; }
        public int Expiry { get; set; }
        public string Token { get; set; }

        public SessionData(string email, string authenticationType, int expiry,
                           string token)
        {
            Email = email;
            AuthenticationType = authenticationType;
            Expiry = expiry;
            Token = token;
        }
    }

    public class RedisClient
    {
        private String hostname;
        private String password;
        private int port;
        private ConnectionMultiplexer? redisClient;
        private log4net.ILog logger;
        private IDatabase? redisDatabase;

        public RedisClient(log4net.ILog loggerInstance,
                           String hostname,
                           int port,
                           String? password = null)
        {
            this.hostname = hostname;
            this.password = password ?? String.Empty;
            this.port = port;
            this.redisClient = null;
            this.logger = loggerInstance;
        }

        public bool Initialise()
        {
            bool status = false;

            var options = ConfigurationOptions.Parse($"{hostname}:{port}");
            if (password != String.Empty)
            {
                options.Password = password;
            }

            try
            {
                redisClient = ConnectionMultiplexer.Connect(options);
                redisDatabase = redisClient.GetDatabase();
                status = true;
            }
            catch (StackExchange.Redis.RedisConnectionException except)
            {
                logger.Fatal($"Unable to connect to redis : {except.Message}");
            }

            return status;
        }

        public async Task<bool> AddAuthSession(String emailAddress,
                                               String token,
                                               AuthenticationType authType)
        {
            /* Add an authentication session to the REDIS database. It will
              attempt to lock the record before adding it to ensure concurrency
              consistency. */

            bool status = false;

            String lockToken = Guid.NewGuid().ToString();

            if (!AcquireLock(emailAddress, lockToken))
            {
                logger.Error($"Unable to get a REDIS lock for {emailAddress}");
            }
            else
            {
                SessionData sessionData = new SessionData(emailAddress,
                                   authType.ToString(), 0, token);

                var json = JsonConvert.SerializeObject(sessionData);

                var isSet = await SetStringAsync(emailAddress, json);
                if (!isSet)
                {
                    logger.Error("Unable to add session data!");
                }
                else
                {
                    ReleaseLock(emailAddress, lockToken);
                    status = true;
                }
             }

            return status;
        }

        private async Task<bool> SetStringAsync(string key, string value,
                                                TimeSpan? timeSpan = null)
        {
            if (redisDatabase == null)
            {
                logger.Error("Redis database isn't connected when trying" +
                             " to call RedisClient::SetStringAsync");
                return false;
            }

            return await redisDatabase.StringSetAsync(key, value, timeSpan);
        }

        private bool AcquireLock(String key, String lockToken, int timeoutMs = 2,
                                 int durationMs = 1)
        {
            /* Acquire a lock, retrying for timeout number of seconds. */

            bool lockAcquired = false;
            int currentTimeMs = DateTime.Now.Millisecond;
            int endTimeMsg = currentTimeMs + timeoutMs;
            TimeSpan durationTimespan = TimeSpan.FromMilliseconds(durationMs);

            while (DateTime.Now.Millisecond < endTimeMsg && !lockAcquired)
            {
                lockAcquired = redisDatabase.LockTake(key, lockToken, durationTimespan);
                if (!lockAcquired)
                {
                    Thread.Sleep(1);
                }
            }

            return lockAcquired;
        }

        private bool ReleaseLock(string EmailAddress, string lockToken)
        {
            /*
            Attempt to release a lock. If the lock name doesn't exist or the
            identifier doesn't match then an exception is raised.
            */

            bool status = redisDatabase.LockRelease(EmailAddress, lockToken);

            if (!status)
            {
                logger.Error("Failed attempt releasing REDIS lock with id " +
                             $"for session '{EmailAddress}");
            }

            return status;
        }
    }
}


//def del_auth_session(self, email_address : str) -> bool:
//"""
//Remove an authentication session. It will attempt to lock the record
//before deleting it to ensure concurrency consistency.
//parameters:
//email_address - Email address of the user
//returns:
//bool - Success status of the del.
//"""

//status = False

//lock_id = self._acquire_lock(email_address)
//if not lock_id:
//    self._logger.error("Unable to get a REDIS lock for %s",
//               email_address)

//else:
//# If you logon a second time it will just invalid the previous one
//# currently. This logic should be improved at a later date!!
//self._redis.delete(email_address)
//self._release_lock(email_address, lock_id)
//status = True

//return status

//def is_valid_session(self, email_address : str, token: str) -> bool:
//"""
//Verify if a session token for a given email address is valid.
//parameters:
//email_address - Email address of the user\n
//token - Token value
//returns:
//bool - Validity boolean.
//"""

//status = False

//session = self._redis.get(email_address)

//if session:
//json_data = json.loads(session)
//recv_token = json_data["token"]
//status = recv_token == token

//return status

//def has_session(self, email_address : str) -> bool:
//"""
//Verify if a session exits for a given email address.
//parameters:
//email_address - Email address of the user\n
//returns:
//bool - Validity boolean.
//"""

//session = self._redis.get(email_address)
//return session is not None

// THIS IS 224