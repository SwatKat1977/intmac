import re


RELEASE_REGEX = re.compile(r"""^RELEASE-\d{1,3}\.\d{1,3}\.\d{1,3}$""",
                           re.VERBOSE | re.IGNORECASE)

print(RELEASE_REGEX)

branch = "ReLEASE-1.0.0"

release_matches = RELEASE_REGEX.findall(branch)
if release_matches:
    print(release_matches[0])
