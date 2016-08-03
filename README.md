# LC2 Service Specification

## Changelog
 * **2016-07-29**:
   * Initial Draft

## Notes
The words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" are used in accordance to [RFC2119](https://www.ietf.org/rfc/rfc2119.txt).

## Introduction
The first part of this specification is intended as a complete set of the minimum service requirements (==LC2-MIN==) to register and run a service using the [LUCI2](https://bitbucket.org/treyerl/luci2) middleware. It includes a definition of the LC2 protocols and mechanisms as well as a set of example calls a service makes throughout its lifetime.

The second part extends this initial set of requirements to achieve compliance with the [Quick Urban Analysis Kit](https://github.com/achirkin/qua-kit) (==LC2-QUA==). While the original LC2-MIN specification can be considered *weak* in a sense that it allows for very powerful general-purpose services, QUA-services are necessarily interchangeable across visualization techniques and input scopes. This interoperability is achieved by extending the original data model and by specifying a domain-specific interface for geometry processing services.

## Service Infrastructure
LC2 can be perceived as an IaaS provider, mediating between clients and *cloud* services for computational architecture analysis. Like most IaaS providers Luci's services fall into three different categories: ==storage==, ==networking== and ==computing==. We will broadly outline the main purpose of these functional units within the LC2 framework.

### Storage
Luci provides functions to clients and services for storing network-wide data. However, compared to common IaaS storage systems, LC2 focusses on geospatial and geometric data. The storage is immutable, changes to the data are realized using **forward-incremental version control without synchronization** (?).
 * The database is based on what? relational? NoSQL?
  * Relational? NoSQL?
  * Schema requirements?
  * Concurrent read / write? (ACID?)
 * Mutiple storage nodes?
 * Replication strategy?
 * Database Indices? ...

### Networking
All communication in Luci is performed through a single broker, namely, the Luci server. Luci will validate the messages and, if valid, forward them to the designated service or client.
 * Any advanced networking strategy?
 * Scheduling?
 * Addressing?
 * ...

### Computing
In the current state, service actions are delegated to a free computing service meeting the client-specified requirements. If there are no free computing services, the scheduler will [...]?

#### LC2-MIN Specification
TODO
```
+---------+             +-------+              +---------+
| Service |             | Luci  |              | Client  |
+---------+             +-------+              +---------+
     | <metadata>           |                       |
     |--------------------->|                       |
     |                      |               <input> |
     |                      |<----------------------|
     |              <input> |                       |
     |<---------------------|                       |
     | <output>             |                       |
     |--------------------->|                       |
     |                      | <output>              |
     |                      |---------------------->|

```

#### LC2-QUA Specification
##### Additional Metadata
QUA compliant services have one additional required field (**qua-compliant**) and one previously optional field becomes required (**description**). Furthermore, the optional field **constraints** is introduced:
```

{

	"serviceName" : [...],
	"inputs" : [...],
	"outputs" : [...],
	"exampleCall" : [...],
	"qua-compliant" : [...],
	"description" : [...],
	(optional) "constraints" : [...],
	(optional) "serviceId" : [...]
}

```
Constraints restrict a service's input space. They can take two forms - Either a **list** of values corresponding to an input field's type, or, for numeric values, a **range**. If no constraint is given for an input field, the input field's domain is considered unrestricted and every value of it's type is valid. There is a maximum of **one constraint per input field** allowed. If more than one constraint exists for a given input field, the service is considered broken and must be rejected.
```
{
	[...]
	"inputs" : {
		"some input" : "string",
		"some other input" : "number",
		"some third input" : "number"
	},
	[...]
	"constraint" : {
		"some input" : ["foo", "bar"],
		"some other input" : {
			"min" : 0,
			"max" : 100,
			"integer" : True
		}
	}
	[...]
}
```
The example above specifieds three inputs for a service. The first input argument is either **"foo"** or **"bar"**. The second one takes any whole number in the interval [0,100]. The third one can be any number since no constraint has been specified.

Note that a range constraint **does not** necessarily contain all three arguments *min*, *max* and *integer*.

##### Additional Inputs
Furthermore, qua-compliant services have two required inputs. **mode** and **scenario_id** are specifeid below. The *mode* describes what entities shall be analyzed by the service, the value *new* depicts a mode in which new geometry is created. If the mode is either *objects* or *points*, a binary attachment must be included in the service call (see Section **Geometry Serialization**)
```
{
	[...]
	"inputs" : {
		"mode" : "string",
		"scenario_id" : "long"
	},
	[...]
	"constraint" : {
		"mode" : ["scenario", "objects", "points", "new"],
	}
	[...]
}
```

##### Output
The output depends upon the specified mode of operation.
###### *district* :
```
{
	"unit" : [...]
	"value" : [...]
}
```

###### *objects* :
```
{
	"unit" : [...]
	"object_ids" : [...]
	"values" : [...]
}

```

###### *points* :
```
{
	"unit" : [...]
	"values" : [...]
}
```

###### *new* :
```
{
	"scenario_id" : [...]
	"timestamp" : [...]
}
```

##### Geometry Serialization
 * **Points** are serialized to a binary file of `float32` values wherein three consecutive values correspond to one three dimensional vector.
 * **Objects** are serialized to a binary file `ulong64` values wherein each value corresponds to one object id contained in the specified scenario.
