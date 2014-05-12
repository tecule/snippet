package com.sinoparasoft.scs.ws.test;

import java.io.IOException;

import org.restlet.resource.ClientResource;
import org.restlet.resource.ResourceException;

import junit.framework.TestCase;

public class PersonResourceTest extends TestCase {
	public void testGetPersonXml() {
		ClientResource client = new ClientResource("http://rhel63t.dev.scs:8080/RestletDemo/person/a/xml");

		try {
			System.out.println(client.get().getText());
		} catch (ResourceException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void testGetPersonJson() {
		ClientResource client = new ClientResource("http://rhel63t.dev.scs:8080/RestletDemo/person/a/json");
		try {
			System.out.println(client.get().getText());
		} catch (ResourceException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
