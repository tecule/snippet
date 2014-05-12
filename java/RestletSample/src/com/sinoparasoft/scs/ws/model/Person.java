package com.sinoparasoft.scs.ws.model;

import javax.xml.bind.annotation.XmlRootElement;

// here we use JAXB data binding 
// other methods parsing of XML data including JDOM, SAX
@XmlRootElement(name="Person")
public class Person {
	private String name;
	private int sex;
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public int getSex() {
		return sex;
	}
	
	public void setSex(int sex) {
		this.sex = sex;
	}
}
