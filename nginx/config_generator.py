from jinja2 import Environment, FileSystemLoader
import json

file_loader = FileSystemLoader("templates")
env = Environment(loader=file_loader)
template = env.get_template("nginx.conf.j2")
  
# Opening JSON file
f = open('apps.json')
apps = json.load(f)
f.close()

###########################
#RENDERING FUNCTION
###########################

def renderTemplates(appName, appDomain, appEndpoint):

    output = template.render(
        domain = appDomain,
        endpoint = appEndpoint
    )
    with open("./conf/" + appName + ".conf", "w") as f:
        f.write(output)

###########################
#APP ITERATIONS
###########################

for app in apps:
    renderTemplates(app["appName"], app["appDomain"], app["appEndpoint"])
