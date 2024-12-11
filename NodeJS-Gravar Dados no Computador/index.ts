import * as mqtt from "mqtt";
import * as fs from "fs";

interface Topicos {
  [chave: string]: string;
}

const TOPICOS: Topicos = {
  "/org_geminataprojeto_case/sensor_corrente": "corrente.csv",
  "/org_geminataprojeto_case/sensor_tensao": "tensao.csv",
  "/org_geminataprojeto_case/sensor_temperatura": "temperatura.csv",
  "/org_geminataprojeto_case/sensor_umidade": "umidade.csv",
};

interface ArquivosAbertos {
  [chave: string]: fs.WriteStream | null;
}

const arquivosAbertos: ArquivosAbertos = {};

function inicializarArquivosCSV(): void {
  for (const topico in TOPICOS) {
    const arquivo = TOPICOS[topico];
    if (!fs.existsSync(arquivo)) {
      fs.createWriteStream(arquivo).write("timestamp,valor\n");
    }
    arquivosAbertos[arquivo] = fs.createWriteStream(arquivo, { flags: "a" });
  }
}

async function main(): Promise<void> {
  const mqttClient = mqtt.connect("mqtt://broker.hivemq.com");
  inicializarArquivosCSV();

  await new Promise((resolve) => setTimeout(resolve, 2000));

  mqttClient.on("connect", () => {
    console.log("Conectado ao broker MQTT com sucesso!");
    Object.values(TOPICOS).forEach((topico) => {
      mqttClient.subscribe(topico);
      console.log(`Inscrito no tópico: ${topico}`);
    });
  });

  mqttClient.on("message", (topic, message) => {
    const valor = message.toString();
    const topico = topic.toString();
    console.log(`Mensagem recebida no tópico ${topico}: ${valor}`);

    if (TOPICOS[topico]) {
      arquivosAbertos[TOPICOS[topico]]!.write(
        `${new Date().toISOString()},${valor}\n`
      );
    }
  });

  console.log("Aguardando mensagens...");

  try {
    await new Promise((resolve) => setTimeout(resolve, Infinity));
  } catch (error) {
    // Timeout
  }

  console.log("Encerrando o programa...");

  Object.values(arquivosAbertos).forEach((handle) => handle!.destroy());
}

main();
