using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class MovementScript : MonoBehaviour
{

    public float velocity;
    public TMP_Text mlText;
    public TMP_Text maxMlText;
    public TMP_Text numberOfHitsText;
    public GameObject bluetoothManager;
    public float prom = 0;
    public float posY = 0;
    public float posYY = 0;

    NativeCodeRunner scriptBlt;

    private Rigidbody2D rb;
    private float verticalVelocity;
    private int numberOfHits = 0;
    public int maxMl = 0;

    private bool mousePressed = false;

    public GameObject windowGraphGameObject;
    private WindowGraph windowGraph;
    private List<int> patientExerciseValues = new List<int>();
    private Dictionary<int, List<int>> patientCompleteExerciseDictionary = new Dictionary<int, List<int>>();
    
    void Start() {
        scriptBlt = bluetoothManager.GetComponent<NativeCodeRunner>();
        windowGraph = FindObjectOfType<WindowGraph>();
        rb = GetComponent<Rigidbody2D>();
        numberOfHitsText.text = numberOfHits.ToString();
        maxMlText.text = maxMl.ToString();
    }










    
    void Update() {
        //Mouse inputs
        if (Input.GetMouseButtonDown(0)) mousePressed = true;
        else if (Input.GetMouseButtonUp(0)) mousePressed = false;
        verticalVelocity = mousePressed ? 1 : -1;
        //Touch input
        /*switch (Input.touchCount) {
            case 0:
                verticalVelocity = 0;
                break;
            case 1:
                verticalVelocity = 1;
                break;
            case 2:
                verticalVelocity = -1;
                break;
            default:
                verticalVelocity = 0;
                break;
        }*/
        //Ml Update
        //prom = (float) scriptBlt.prom * 1000;
        //Debug.LogWarningFormat("promedio: {0}", prom.ToString());
        //posY = GameData.GetAbsolutPositionFromMl(prom) + 0.78f;
        int actualMl = scriptBlt.posGL;
        mlText.text = actualMl.ToString();
        if (actualMl > maxMl) {
            maxMl = actualMl;
            maxMlText.text = maxMl.ToString();
        }
        posY = scriptBlt.posY;
        //Debug.LogWarningFormat("position Y: {0}", posY.ToString());
        transform.position = new Vector3(transform.position.x, posY, transform.position.z);

        //int actualMl = GameData.GetMlPositionFromAbsolut(transform.position.y);
        //transform.position.Set(transform.position.x, posY, transform.position.z);
    }

   /*void FixedUpdate() {
        float fixedVelocity = (verticalVelocity > 0) ? velocity : velocity / 2;
        rb.velocity = Vector2.up * verticalVelocity * fixedVelocity;
    }*/

    public void GetPlayerGraph(int serieCount) {
        windowGraph.ShowGraph(patientExerciseValues, WindowGraph.EXERCISE_PATIENT_SERIE_COLOR);
        patientCompleteExerciseDictionary.Add(serieCount, patientExerciseValues);
        patientExerciseValues = new List<int>();
    }

    public void SavePlayerGraph(int serieCount) {
        patientCompleteExerciseDictionary.Add(serieCount, patientExerciseValues);
        patientExerciseValues = new List<int>();
    }

    public List<GameObject> GetSerieGraph(int serieNumber) {
        return windowGraph.ShowGraph(patientCompleteExerciseDictionary[serieNumber], WindowGraph.EXERCISE_PATIENT_SERIE_COLOR);
    }


    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Obstacle"))
        {
            collision.gameObject.SetActive(false);
            numberOfHits++;
            numberOfHitsText.text = numberOfHits.ToString();
        }
        else if (collision.gameObject.CompareTag("Graph")) {
            patientExerciseValues.Add(GameData.GetMlPositionFromAbsolut(transform.position.y) + (int)GameData.minCapacity);
        }
    }
}
