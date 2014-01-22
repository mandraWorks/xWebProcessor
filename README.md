# xWebProcessor v0.2.0
####License
xWebProcessor is free software distributed under the terms of the MIT license (see LICENSE file).
## Introduction
In contrast to (old school) CMS systems,  modern web application do not rely anymore on the paradigm to generate the 
final complete html page entirely on the server side (like php, asp, jsp , etc.). Furthermore modern web application deliver only realtime data as ditionaries (json files) from a REST webservice and 
realize the rendering of the html pages on the client side (browser, using javascript). This approach is technically very mature due to tremdeous progress of the modern javascript engins (Google V8).
So only realtime data (frequently changed data) is traveling frequently through the network. All other data like translations, menu and page structure and so on, is generated offline and rendered 
on the client side (send only once when loading the html, javascript, css and assests). The modification of this kind of information is managed by the publication and release cycle of the web application. 
To work efficently with this approach, the use a static web 
site generator is recommand in order to handle web page structure and translations, tasks which had been done by the CMS sytem before.

The xWebProcessor is the implementation of such static web site generator based on XML. The author is clearly aware that there are a lot of other static website generators on the market these days.
Why implement another one? Actually it was not really mandatory. Other static web page generators are very powerful and in some areas maybe more efficent than the xWebProcessor. I implemented this tools
for two reasons: First: it was not a lot of effort (approxiamtly 2 days of work), second: It gaved me the possibility to adapte the markup language for the generation exactly to my needs.

## How does it work
The xWebProcessor is a command line tool which has one parameter is input: the project file. The following illustration outlines the basic topologie of the files used during the static web site generation.

![Overview](https://raw.github.com/mandraWorks/xWebProcessor/develop/Doc/Overview.png "xWebProcessor and files")

All data processed by the xWebProcessor is presented as XML (embedded or file). In the following I will present the syntaxt of the this XML dialect (xWebML).
### The project file
I use the follwing extension for the project file: xwebproj.xml. It is not mandatory. In the following illustration you can see the schema o fthe project file.

![Project Schema](https://raw.github.com/mandraWorks/xWebProcessor/develop/Doc/project.png "Project Schema")

### The content files
The content files contains the string list, in general the translations. Currently xWebProcessor supports to formats for the string list:
- xWebStringList files (part of the xWebML)
- strings files (Apple string resource files)

The schema os the xWebML stringlist is as follows.

![String List Schema](https://raw.github.com/mandraWorks/xWebProcessor/develop/Doc/stringlist.png "String List Schema")

### The Templates
#### The html fragments
The html fragments contains html blocks which are assembled by the xWebProcessor to the final web pages. 
These fragments can be injected with XML sections which are processed by the xWebProcessor. in the following I will
present the different XML sections which can be embedded into the html code.

##### Include file
By placing an XML bloc which validates the following schema, you can indicate the xWebProcessor to include another file
(cut and paste).

![Include File Schema](https://raw.github.com/mandraWorks/xWebProcessor/develop/Doc/includefile.png "Include File Schema")

Here a small code example:

```html
</div>
  <div class="span2">
    <xWeb:IncludeFile>
      <File>Templates/actions.html</File>
    </xWeb:IncludeFile>
  </div>
</div>
```

##### Content link *(depcreacted)*
A content link activates a content file which is used in the following text of the html fragment for replacing 
string or content entires with localized strings.

![Content Link Schema](https://raw.github.com/mandraWorks/xWebProcessor/develop/Doc/contentlink.png "Content Link Schema")

Here a small code example:

```html
</head>

<xWeb:ContentLink>
  <Folder>Content/</Folder>
  <FileName>Common.xwebstringlist.xml</FileName>
</xWeb:ContentLink>

<body>
```

##### Content string *(depcreacted)*
Once you activate a content file you can use strings from this file. In the following example you can see 
how to use a string identified by its key in your html fragment.

```html
<div class="citationbody">
  <p class="citationtext justify"><xWeb:Content>FMStatement</xWeb:Content> </p>
  <p class="citationname"><xWeb:Content>FMName</xWeb:Content> </p>
</div>
```


##### Menu
In order to create a modulare and localized menu you need to create two XML entities:

- a menu section in the html fragment
- a link list file

In the following you can see the schema of the XML section which creates a menu in the html fragement.

![Menu Schema](https://raw.github.com/mandraWorks/xWebProcessor/develop/Doc/menu.png "Menu Schema")

An example:

```html
<ul class="dropdown-menu">
  <xWeb:Menu>
    <TemplateFile>Templates/topHomeMenu.tpl</TemplateFile>
    <LinkListFile>Templates/topHomeMenu.xweblinklist.xml</LinkListFile>
  </xWeb:Menu>
</ul>
```

The referenced link list file has the following XML schema:

![Link List Schema](https://raw.github.com/mandraWorks/xWebProcessor/develop/Doc/linklist.png "Link List Schema")

The tpl file defines a ctemplate for the rendering of each menu entry. See the following example for the content of such a tpl file:

```html
<li{{#Act_Sec}} class="active"{{/Act_Sec}}><a href="{{Link}}">{{Label}}</a></li>
```


## Compilation
### Requirements

- **boost 1.54.0** (http://www.boost.org): 
xWebProcessor uses boost for basic string anf file handling and for parsing the command line parameters.
- **xsd 3.3.0** (http://www.codesynthesis.com/products/xsd): 
xWebProcessor uses the excellent xsd tool in order to generated the xml schema - c++ bindings for xWebML (xWebMarkuplangauge).
- **ctemplate 2.2** (http://code.google.com/p/ctemplate): 
xWebProcessor uses the very efficient ctemplate library form Google for the basic templating, for example menu item generation.

The xWebProcessor is compiled using cmake. Run cmake and enter the necessary path in order to find the required libraries listed above.


## Examples
The following web sites a rerealized with the xWebProcess. Feel free to browse the sources in order to evaluate the xWebProcessor.

### SFRF

Link: http://www.sfrf.ch

Sources: https://github.com/mandraWorks/SFRFweb_dev

### PSPsmartsoft

Link: http://www.pspsmartsoft.com

Sources: n.a. (for the moment)

### Mandraworks

Link: www.mandraworks.ch

Sources: https://github.com/mandraWorks/mandraworks_dev
