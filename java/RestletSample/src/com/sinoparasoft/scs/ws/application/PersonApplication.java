package com.sinoparasoft.scs.ws.application;

import java.util.HashSet;
import java.util.Set;

import javax.ws.rs.core.Application;
import com.sinoparasoft.scs.ws.resource.PersonResource;

// multiple resources can be manipulated by one application
public class PersonApplication extends Application {
	@Override
	public Set<Class<?>> getClasses() {
		Set<Class<?>> resources = new HashSet<Class<?>>();
        resources.add(PersonResource.class);
        return resources;
	}
}
