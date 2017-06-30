# What are minions #
Minions are small helper scripts, comments, to-dos, which help us in carrying out most mundane, but often forgettable tasks.

## Object Storage Minions ##
1. Create Object Storage service in Bluemix  
2. Generate Auth_Token
<pre><code>curl -D Auth_Token.txt -X POST -H "Content-Type: application/json" -d '{ "auth":{ "identity":{ "methods": ["password"], "password":{"user":{"id": "[userId from credentials json]", "password": "[password from credentials json]"} }}, "scope":{ "project":{"id": "[projectId from credentials json]"}}}}' 'https://identity.open.softlayer.com/v3/auth/tokens'</code></pre>
3. Retrieve the object using GET request
<pre><code>curl --header "X-Auth_Token:[X-Auth Token obtained above]" https://dal.objectstorage.open.softlayer.com/v1/AUTH_[projectId]/[container]/[object to be retrieved] -o [full path on local system]</code></pre>

