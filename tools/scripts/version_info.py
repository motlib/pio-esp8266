#!/usr/bin/env python

'''Script to generate a header containing one define with the current build date
in YYYYMMDD-hhmm format and the git hash.'''

from datetime import datetime
from subprocess import check_output

header_file='src/diag/version_info.h'

print "Generating {header_file}".format(header_file=header_file)

tmpl = '''
#ifndef VERSION_INFO_H
#define VERSION_INFO_H

#define VERSION_BUILD_DATE "{build_date}"
#define VERSION_GIT_HASH "{git_hash}"

#endif /* VERSION_INFO_H */'''

build_date = datetime.utcnow().strftime("%Y%m%d-%H%M")
git_hash = check_output(['git', 'rev-parse', '--verify', 'HEAD']).strip()

with open(header_file, 'w') as f:
    f.write(tmpl.format(build_date=build_date, git_hash=git_hash))
    
