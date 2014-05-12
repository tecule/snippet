package com.sinoparasoft.scs.ws.resource;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;

import com.sinoparasoft.scs.ws.model.Person;

// uri path including person will be processed by PersonResource class
@Path("person")
public class PersonResource {
	// HTTP Method
	@GET
	// uri path
	@Path("{name}/xml")
	// return data format
	@Produces("application/xml")
	public Person getPersonXml(@PathParam("name") String name) {
		Person person = new Person();
		person.setName("Steven");
		person.setSex(1);
		return person;
	}

	@GET
	@Path("{name}/json")
	@Produces("application/json")
	public Person getPersonJson(@PathParam("name") String name) {
		Person person = new Person();
		person.setName("Steven");
		person.setSex(1);
		return person;
	}

}
