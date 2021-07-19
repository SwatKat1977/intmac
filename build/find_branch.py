'''
Copyright 2014-2021 Integrated Test Management Suite

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
'''
import re
import sys

dev_branches = ['project-restart', 'development', 'master']

RELEASE_REGEX = re.compile(r"""^RELEASE-\d{1,3}\.\d{1,3}\.\d{1,3}$""",
                           re.VERBOSE | re.IGNORECASE)

BRANCH_REGEX = re.compile(r"""^(?:task\/|bug\/)(IT-\d{1,3}\d{1,3})""",
                          re.VERBOSE | re.IGNORECASE)

def main():
    """ """

    if len(sys.argv) != 2:
        print('[ERROR] Invalid umber of arguments passed to find_branch.py',
              file = sys.stderr)
        return

    branch = sys.argv[1]
    branch_tag = "UNKNOWN"

    release_matches = RELEASE_REGEX.findall(branch)
    if release_matches:
        branch_tag = release_matches[0].upper()

    release_matches = BRANCH_REGEX.findall(branch)
    if release_matches:
        branch_tag = release_matches[0].upper()

    if branch in dev_branches:
        branch_tag = "DEV"

    print(branch_tag)

main()