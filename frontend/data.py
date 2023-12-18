import requests

serverAddr = "http://localhost:8080"


def get(addr):
    try:
        return requests.get(serverAddr + addr).json()
    except Exception:
        return


def post(addr, obj=None):
    requests.post(url=serverAddr + addr, json=obj)