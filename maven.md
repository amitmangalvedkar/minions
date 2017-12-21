# Maven in 1 day #

Simplest pom.xml file
<pre><code>
<project>
  <modelVersion>4.0.0</modelVersion>
  <groupId>org.sonatype.mavenbook</groupId>
  <artifactId>my-project</artifactId>
  <version>1.0</version>
</project>
</code></pre>
modelVersion should **always be** 4.0.0
groupId example com.company.bank
artifactId example consumer-banking

To generate the artifacts run
<pre><code>
mvn install
</code></pre>


*But then what does this do*
<pre><code>
mvn archetype:create -DgroupId=org.sonatype.mavenbook.ch03 -DartifactId=simple -DpackageName=org.sonatype.mavenbook
</code></pre>

This also generates index.html in target/site pointing to javadoc

If you want to write and execute unit tests, all you need to do is write the tests, place them in ${basedir}/src/test/java, add a test-scoped dependency on either TestNG or JUnit, and run mvn test.
If you want to deploy a web application and not a JAR, all you need to do is change you project type to WAR and put your docroot in ${basedir}/src/main/webapp.

<pre><code>
<table>
<tr><th>Item</th><th>Default</th></tr>
<tr><td>source code	</td><td>${basedir}/src/main/java </td></tr>
<tr><td>resources </td><td>${basedir}/src/main/resources </td></tr>
<tr><td>Tests </td><td>${basedir}/src/test </td></tr>
<tr><td>distributable JAR </td><td>	${basedir}/target </td></tr>
<tr><td>Complied byte code </td><td>${basedir}/target/classes </td></tr>
</table>
<code></pre>
