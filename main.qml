import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import HTTP 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: "POST/GET Testes"

    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: true
    }

    Button {
        text: "Loader"
        anchors.centerIn: parent
        onClicked: http.onLoad()
    }

    Http {
        id: http
        Component.onCompleted: onLoad()
        onPostFinish: {
            busy.visible = false
            console.log("Ok")
        }
        onPostError: {
            busy.visible = false
            console.log(errorCode, errorMessage)
        }
        onGetError: {
            busy.visible = false
            console.log(errorCode, errorMessage)
        }
        onGetFinish: {
            busy.visible = false

            for (var i in data) {

                var obj = data[i]
                console.log(obj.nom)
            }
        }
        function onLoad() {

            busy.visible = true

            // modelo de dados
            var operadoras = [
                {"nome":"OI", "codigo":"14"},
                {"nome":"Vivo", "codigo":"15"},
                {"nome":"Tim", "codigo":"41"}
            ]

            // modelo de solicitação de dados
            var conexao = [
                {"login":"vicente",
                 "password":"123",
                 "class":"Teste",
                 "func":"getmotoristas"}
            ]

            // salva o modelo em arquivo
            var resp = saveJSon("package.data", JSON.stringify(conexao))
            if (!resp) {
                return
            }

            // requisita os dados
            get("package.data")
        }
    }
}
