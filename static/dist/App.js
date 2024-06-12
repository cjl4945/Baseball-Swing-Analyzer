import React from "../_snowpack/pkg/react.js";
import {useEffect, useState} from "../_snowpack/pkg/react.js";
import {io} from "../_snowpack/pkg/socket.io-client.js";
const socket = io();
export default function App({}) {
  const [isLoading, setIsLoading] = useState(false);
  const [loadingMessage, setLoadingMessage] = useState("");
  const [hipMetric, setHipMetric] = useState();
  const [shoulderMetric, setShoulderMetric] = useState();
  useEffect(() => {
    socket.on("connect", () => console.log("Connected"));
    socket.on("disconnect", () => console.log("Diconnected"));
    socket.on("calibrate", () => setTimeout(() => {
      setIsLoading(false);
    }, 2e3));
    socket.on("metrics", ({hipData, shoulderData}) => {
      setTimeout(() => {
        setHipMetric(hipData);
        setShoulderMetric(shoulderData);
        setIsLoading(false);
      }, 2e3);
    });
    return () => {
      socket.off("connect");
      socket.off("calibrate");
      socket.off("metrics");
      socket.off("disconnect");
    };
  }, []);
  const startMetrics = () => {
    setLoadingMessage("Getting data...");
    setIsLoading(true);
    socket.emit("start");
  };
  const startCalibrate = () => {
    setLoadingMessage("Calibrating...");
    setIsLoading(true);
    socket.emit("calibrate");
  };
  const Metric = ({type, input}) => {
    if (input === 0) {
      return /* @__PURE__ */ React.createElement("div", {
        className: "p-10 rounded bg-gray-700 flex flex-col justify-center items-center"
      }, /* @__PURE__ */ React.createElement("p", {
        className: "text-white"
      }, type, " ", /* @__PURE__ */ React.createElement("span", {
        className: "font-bold"
      }, "No Data")));
    } else if (input >= 1 && input <= 3) {
      return /* @__PURE__ */ React.createElement("div", {
        className: "p-10 rounded bg-red-500 flex flex-col justify-center items-center"
      }, /* @__PURE__ */ React.createElement("p", {
        className: "text-white"
      }, type, " ", /* @__PURE__ */ React.createElement("span", {
        className: "font-bold"
      }, input)));
    } else if (input >= 4 && input <= 6) {
      return /* @__PURE__ */ React.createElement("div", {
        className: "p-10 rounded bg-yellow-500 flex flex-col justify-center items-center"
      }, /* @__PURE__ */ React.createElement("p", {
        className: "text-white"
      }, type, " ", /* @__PURE__ */ React.createElement("span", {
        className: "font-bold"
      }, input)));
    } else {
      return /* @__PURE__ */ React.createElement("div", {
        className: "p-10 rounded bg-green-500 flex flex-col justify-center items-center"
      }, /* @__PURE__ */ React.createElement("p", {
        className: "text-white"
      }, type, " ", /* @__PURE__ */ React.createElement("span", {
        className: "font-bold"
      }, input)));
    }
  };
  return /* @__PURE__ */ React.createElement("div", {
    className: "w-full h-screen bg-white flex flex-col justify-center items-center"
  }, isLoading && /* @__PURE__ */ React.createElement("div", {
    className: "w-full h-screen bg-gray-700/75 absolute flex flex-col gap-2 justify-center items-center"
  }, /* @__PURE__ */ React.createElement("h1", {
    className: "text-2xl text-white"
  }, loadingMessage), /* @__PURE__ */ React.createElement("div", {
    role: "status"
  }, /* @__PURE__ */ React.createElement("svg", {
    "aria-hidden": "true",
    className: "mr-2 w-20 h-20 text-gray-200 animate-spin dark:text-gray-600 fill-blue-600",
    viewBox: "0 0 100 101",
    fill: "none",
    xmlns: "http://www.w3.org/2000/svg"
  }, /* @__PURE__ */ React.createElement("path", {
    d: "M100 50.5908C100 78.2051 77.6142 100.591 50 100.591C22.3858 100.591 0 78.2051 0 50.5908C0 22.9766 22.3858 0.59082 50 0.59082C77.6142 0.59082 100 22.9766 100 50.5908ZM9.08144 50.5908C9.08144 73.1895 27.4013 91.5094 50 91.5094C72.5987 91.5094 90.9186 73.1895 90.9186 50.5908C90.9186 27.9921 72.5987 9.67226 50 9.67226C27.4013 9.67226 9.08144 27.9921 9.08144 50.5908Z",
    fill: "currentColor"
  }), /* @__PURE__ */ React.createElement("path", {
    d: "M93.9676 39.0409C96.393 38.4038 97.8624 35.9116 97.0079 33.5539C95.2932 28.8227 92.871 24.3692 89.8167 20.348C85.8452 15.1192 80.8826 10.7238 75.2124 7.41289C69.5422 4.10194 63.2754 1.94025 56.7698 1.05124C51.7666 0.367541 46.6976 0.446843 41.7345 1.27873C39.2613 1.69328 37.813 4.19778 38.4501 6.62326C39.0873 9.04874 41.5694 10.4717 44.0505 10.1071C47.8511 9.54855 51.7191 9.52689 55.5402 10.0491C60.8642 10.7766 65.9928 12.5457 70.6331 15.2552C75.2735 17.9648 79.3347 21.5619 82.5849 25.841C84.9175 28.9121 86.7997 32.2913 88.1811 35.8758C89.083 38.2158 91.5421 39.6781 93.9676 39.0409Z",
    fill: "currentFill"
  })))), /* @__PURE__ */ React.createElement("div", {
    className: "w-full flex flex-col justify-center items-center gap-y-4"
  }, /* @__PURE__ */ React.createElement("div", {
    className: "w-full text-center"
  }, /* @__PURE__ */ React.createElement("h1", {
    className: "text-3xl"
  }, "GROUP P22071 HITTING ANALYSIS")), /* @__PURE__ */ React.createElement("div", {
    className: "w-full flex gap-4"
  }, /* @__PURE__ */ React.createElement("div", {
    className: "w-1/2 bg-orange-500 text-white font-bold flex justify-center items-center py-2 cursor-pointer hover:bg-gray-700",
    onClick: startMetrics
  }, "Start"), /* @__PURE__ */ React.createElement("div", {
    className: "w-1/2 bg-orange-500 text-white font-bold flex justify-center items-center py-2 cursor-pointer hover:bg-gray-700",
    onClick: startCalibrate
  }, "Calibrate"), "       "), /* @__PURE__ */ React.createElement("h1", {
    className: "text-xl"
  }, "Metrics"), /* @__PURE__ */ React.createElement("div", {
    className: "w-full grid grid-cols-4 gap-4"
  }, /* @__PURE__ */ React.createElement(Metric, {
    type: "Hip Rotation",
    input: hipMetric?.hipRotation ?? 0
  }), /* @__PURE__ */ React.createElement(Metric, {
    type: "Hip Velocity",
    input: hipMetric?.hipVelocity ?? 0
  }), /* @__PURE__ */ React.createElement(Metric, {
    type: "Shoulder Rotation",
    input: shoulderMetric?.shoulderRotation ?? 0
  }), /* @__PURE__ */ React.createElement(Metric, {
    type: "Shoulder Velocity",
    input: shoulderMetric?.shoulderVelocity ?? 0
  })), /* @__PURE__ */ React.createElement("div", {
    className: "w-full bg-orange-500 text-white font-bold flex justify-center items-center py-2 cursor-pointer hover:bg-gray-700",
    onClick: () => alert("Redirect to metrics explained")
  }, "Metrics Explained")));
}
