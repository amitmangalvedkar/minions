## Object Storage Minions ##
In case of you are in a dilemma, refer to [OpenStack API Reference](https://developer.openstack.org/api-ref/object-storage/)
1. Create Object Storage service in Bluemix
2. Generate Auth_Token
<pre><code>curl -D Auth_Token.txt -X POST -H "Content-Type: application/json" -d '{ "auth":{ "identity":{ "methods": ["password"], "password":{"user":{"id": "[userId from credentials json]", "password": "[password from credentials json]"} }}, "scope":{ "project":{"id": "[projectId from credentials json]"}}}}' 'https://identity.open.softlayer.com/v3/auth/tokens'</code></pre>
3. Retrieve an object using GET request
<pre><code>curl --header "X-Auth_Token:[X-Auth Token obtained above]" https://dal.objectstorage.open.softlayer.com/v1/AUTH_[projectId]/[container]/[object to be retrieved] -o [full path on local system]</code></pre>
4. Create (or replace) an object using PUT request
<pre><code>curl -H "X-Auth_Token:[X-Auth Token obtained above]" -X PUT  https://dal.objectstorage.open.softlayer.com/v1/AUTH_[projectId]/[container]/[object to be create] -d "[content of the file]" -H "Content-Type: text/html; charset=UTF-8"</code></pre>
For Binary files
<pre><code>curl -H "X-Auth_Token:[X-Auth Token obtained above]" -X PUT  https://dal.objectstorage.open.softlayer.com/v1/AUTH_[projectId]/[container]/[object to be create] -T "/tmp/amit.jpg" -H "Content-Type: application/octet-stream"</code></pre>
