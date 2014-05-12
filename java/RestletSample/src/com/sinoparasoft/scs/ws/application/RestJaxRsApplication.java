package com.sinoparasoft.scs.ws.application;

import org.restlet.Context;
import org.restlet.ext.jaxrs.JaxRsApplication;

// initialize JAX-RS web service runtime, multiple applications can be added here
public class RestJaxRsApplication extends JaxRsApplication {
	public RestJaxRsApplication(Context context) {
        super(context);
        this.add(new PersonApplication());
    }	
}
