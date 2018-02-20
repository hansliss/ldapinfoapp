# ldapinfoapp
A simple PHP hack to show some LDAP info from AD, and to expand group
membership for given groups

Will show selected LDAP info for the logged-in REMOTE_USER.

Should be provided with a basedn and filter for finding suitable groups,
and provide an option to show an expanded list of members for each group.

This can be tailored specifically to AD, and so use either "member" or
"memberOf" or both, as needed.