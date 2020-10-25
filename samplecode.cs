
#region Private Serializable Fields

/// <summary>
/// The maximum number of players per room
/// </summary>
[Tooltip("The maximum number of players per room. New room will be created if room is full")]
[SerializeField]
private byte maxPlayersPerRoom = 10;

#endregion





//avatar manager for updating transform of avatar prefab
public class AvatarManager : MonoBehaviourPun, Photon.Pun.IPunObservable
{
    public GameObject avatar;
    public Transform playerGlobal, playerLocal;
    private PhotonView PV;

    void Start()
    {
        Debug.Log("AvatarManager: an avatar instantiated");
        PV = GetComponent<PhotonView>();

        //if this is my photonview, set transform of game object
        if (PV.IsMine)
        {
            Debug.Log("player is mine");

            //find my game object
            playerGlobal = GameObject.Find("OVRPlayerController").transform;
            playerLocal = playerGlobal.Find("OVRCameraRig/TrackingSpace/CenterEyeAnchor");

            //set transform parent
            this.transform.SetParent(playerLocal);
            this.transform.localPosition = new Vector3(0f, -1f, -1.4f);
        }
    }

    //stream position and rotation
    void IPunObservable.OnPhotonSerializeView(PhotonStream stream, PhotonMessageInfo info)
    {
        if (stream.IsWriting)
        {
            stream.SendNext(playerGlobal.position);
            stream.SendNext(playerGlobal.rotation);
            stream.SendNext(playerLocal.localPosition);
            stream.SendNext(playerLocal.localRotation);
        }
        else
        {
            this.transform.position = (Vector3)stream.ReceiveNext();
            this.transform.rotation = (Quaternion)stream.ReceiveNext();
            avatar.transform.localPosition = (Vector3)stream.ReceiveNext();
            avatar.transform.localRotation = (Quaternion)stream.ReceiveNext();
        }
    }
}
