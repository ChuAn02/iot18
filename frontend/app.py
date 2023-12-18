from flask import Flask, render_template, request
import data

app = Flask(__name__)
app.secret_key = "iot"


# home


@app.route("/", methods=["GET", "POST"])
def home():
    if request.method == "GET":
        fanData = data.get("/api/fan")
        lightData = data.get("/api/light")
        temphumData = data.get("/api/temphum")
        return render_template(
            "index.html", fan=fanData, light=lightData, temphum=temphumData
        )
    else:
        path = "/api" + request.data.decode()
        data.post(path)
        return ""


# light


@app.route("/light")
def light():
    lightData = data.get("/api/light")
    lightChart = data.get("/api/light/chart")
    return render_template("light.html", light=lightData, chart=lightChart)


@app.route("/light/schedule", methods=["POST"])
def setLightSchedule():
    path = "/api/light/schedule"
    obj = request.data.decode()
    data.post(path, obj)
    return ""


# fan


@app.route("/fan")
def fan():
    fanData = data.get("/api/fan")
    fanChart = data.get("/api/fan/chart")
    return render_template("fan.html", fan=fanData, chart=fanChart)


@app.route("/fan/setting", methods=["POST"])
def setAutoSetting():
    path = "/api/fan/setting"
    obj = request.data.decode()
    data.post(path, obj)
    return ""


# main


if __name__ == "__main__":
    app.run(debug=True)
