mergeInto(LibraryManager.library, {
    getLocalTilemapFile: async function() {
        const pickerOpts = {
            types: [
                {
                    description: 'Image',
                    accept: {
                        'image/*': ['.jpeg', '.jpg', '.png']
                    }
                },
            ],
            excludeAcceptAllOption: true,
            multiple: false,
            _preferPolyfill: false // default
        };
        try {
            // Always returns an array.
            const [handle] = await window.showOpenFilePicker(pickerOpts);
            const file = await handle.getFile();
            const contents = await file.arrayBuffer();
            Module.openTilemapFile(file.name, file.type, contents, contents.byteLength);
            //result = Module.ccall("int_sqrt", null, ["string", "array", "number"], [file.name, uint8View, contents.byteLength])

            //Module.msgDebug(file);
            console.log(file);
            
            return 0;
    
            // return handle.getFile();
        } catch (err) {
            console.error(err.name, err.message);
            Module.msgError(err.message);
        } finally {
            // To avoid memory leaks we need to always clear out the allocated heap data
            // This needs to happen in the finally block, otherwise thrown errors will stop code execution before this happens
            //Module._free(buffer)
        }
        // Do something with the file handle.

        return 1;
    },
    saveLocalFile: async function() {
        try {
            var myUint8Array = Module.getBytes();
            //var blob = new Blob(myUint8Array, {type: "text/json"});
            var string = new TextDecoder().decode(new Uint8Array(myUint8Array));
            // create a new handle
            handle = await window.showSaveFilePicker({
                suggestedName: 'preset.json'
            });

            // create a FileSystemWritableFileStream to write to
            const writableStream = await handle.createWritable();

            await writableStream.write(string);
            await writableStream.close();

            // write our file
            //await writableStream.write(myUint8Array);
            //console.log(blob);

            // close the file and write the contents to disk.
            // await writableStream.close();
            // Module.message("Saving file: " + handle.name);
        } catch (err) {
            console.error(err.name, err.message);
            Module.msgError(err.message);
        } finally {
            // To avoid memory leaks we need to always clear out the allocated heap data
            // This needs to happen in the finally block, otherwise thrown errors will stop code execution before this happens
            //Module._free(buffer)
        }

        return 1;
    },
    viewFullscreen: function() {
        var elem = document.getElementById("canvas");
        if (elem.requestFullscreen) {
            elem.requestFullscreen();
        } else if (elem.webkitRequestFullscreen) { /* Safari */
            elem.webkitRequestFullscreen();
        } else if (elem.msRequestFullscreen) { /* IE11 */
            elem.msRequestFullscreen();
        }

        return 1;
    },
});