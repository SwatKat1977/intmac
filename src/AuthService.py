#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import argparse
from ServerServices.AuthService.AuthService import *


def main():
	argsParser = argparse.ArgumentParser()

	# 'Optional Arguments' : 'config'.
	argsParser.add_argument('-c', '--config', required = False, 
		help = 'Configuration file to read.')

	# Parse the arguments.
	args = argsParser.parse_args()

	# If a configuration file is specified then set on the gateway.  
	if args.config != None:
		AuthenticationService.ConfigFile = args.config

	AuthenticationService.Run()


if __name__ == "__main__":
	main()
